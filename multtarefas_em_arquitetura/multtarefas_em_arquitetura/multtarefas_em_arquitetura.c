#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/adc.h"
#include "hardware/irq.h"
#include "hardware/pwm.h"
#include "pico/multicore.h"

// Definições
#define LIMIAR_ESTADO 70  // Limiar para emergência

#define PINO_JOYSTICK_X 26
#define PINO_JOYSTICK_Y 27

#define PINO_LED_R 13
#define PINO_LED_G 11
#define PINO_LED_B 12

#define PINO_BUZZER 10

// Variáveis globais
volatile uint8_t flag_estado = 0;

// Função para ler o joystick e atualizar flag_estado
void ler_joystick() {
    uint16_t leitura_x = adc_read();
    adc_select_input(1); // Troca para canal Y
    uint16_t leitura_y = adc_read();
    adc_select_input(0); // Volta para canal X
    
    uint16_t media = (leitura_x + leitura_y) / 2;
    flag_estado = (uint8_t)((media * 100) / 4095); // Escala para 0-100
}

// Função de callback do alarme para enviar o flag via FIFO
int64_t enviar_estado_fifo(alarm_id_t id, void *user_data) {
    multicore_fifo_push_blocking(flag_estado);
    printf("[Core0] Estado enviado: %d\n", flag_estado);
    return 2000000; // 2 segundos em microssegundos
}

// Configura PWM para controlar cor do LED
void configurar_pwm_led(uint led_pin) {
    gpio_set_function(led_pin, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(led_pin);
    pwm_set_wrap(slice, 65535);
    pwm_set_enabled(slice, true);
}

void set_cor_led(uint8_t estado) {
    uint16_t intensidade_r = 0, intensidade_g = 0, intensidade_b = 0;

    if (estado < 30) {
        intensidade_g = 50000; // Verde
    } else if (estado < 70) {
        intensidade_r = 30000; intensidade_g = 30000; // Amarelo
    } else {
        intensidade_r = 50000; // Vermelho
    }

    pwm_set_gpio_level(PINO_LED_R, intensidade_r);
    pwm_set_gpio_level(PINO_LED_G, intensidade_g);
    pwm_set_gpio_level(PINO_LED_B, intensidade_b);
}

// Função que rodará no Core1
void core1_main() {
    while (true) {
        
        uint32_t estado_recebido = multicore_fifo_pop_blocking();
        printf("[Core1] Estado recebido: %d\n", estado_recebido);

        // Ajusta LED
        set_cor_led((uint8_t)estado_recebido);

        // Ativa ou desativa buzzer
        if (estado_recebido > LIMIAR_ESTADO) {
            gpio_put(PINO_BUZZER, 1);
        } else {
            gpio_put(PINO_BUZZER, 0);
        }
    }
}

// Função que rodará no Core1

int main()
{
    stdio_init_all(); 

    // Inicializa GPIOs
    gpio_init(PINO_LED_R); gpio_init(PINO_LED_G); gpio_init(PINO_LED_B);
    gpio_init(PINO_BUZZER);
    gpio_set_dir(PINO_BUZZER, GPIO_OUT);

    configurar_pwm_led(PINO_LED_R);
    configurar_pwm_led(PINO_LED_G);
    configurar_pwm_led(PINO_LED_B);

    // Inicializa ADC
    adc_init();
    adc_gpio_init(PINO_JOYSTICK_X);
    adc_gpio_init(PINO_JOYSTICK_Y);
    adc_select_input(0); // Começa lendo X

    // Inicializa Core 1
    multicore_launch_core1(core1_main);

    // Configura alarme para enviar estado a cada 2 segundos
    add_alarm_in_us(2000000, enviar_estado_fifo, NULL, true);

    while (true) {

        ler_joystick();
        sleep_ms(100); // Pequeno delay para estabilidade
        // printf("Hello, world!\n");
        // sleep_ms(1000);
    }
}
