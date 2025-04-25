#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
#include "pico/multicore.h"
#include "pico/time.h"
#include "hardware/irq.h"
#include "hardware/sync.h"

// === Configurações ===
#define JOYSTICK_ADC_CHANNEL 0  // ADC0 para eixo X
#define LED_R 2
#define LED_G 3
#define LED_B 4
#define BUZZER_PIN 5

#define LIMIAR_CRITICO 3000  // Defina conforme o ADC (0-4095)

// === Estado Global ===
volatile uint16_t flag_estado = 0;

// === Função do Alarme (Core 0) ===
int64_t alarme_callback(alarm_id_t id, void *user_data) {
    multicore_fifo_push_blocking(flag_estado);
    return 2000; // chama novamente após 2000ms
}

// === Core 0 ===
void core0_main() {
    adc_init();
    adc_gpio_init(26); // GPIO26 = ADC0
    adc_select_input(JOYSTICK_ADC_CHANNEL);

    // Configura alarme periódico
    add_alarm_in_ms(2000, alarme_callback, NULL, true);

    while (1) {
        flag_estado = adc_read(); // Lê joystick
        sleep_ms(100); // Pequeno delay
    }
}

// === Função para ajustar cor RGB ===
void ajustar_rgb(uint16_t estado) {
    if (estado < 1000) {
        gpio_put(LED_R, 0); gpio_put(LED_G, 1); gpio_put(LED_B, 0); // Verde
    } else if (estado < LIMIAR_CRITICO) {
        gpio_put(LED_R, 1); gpio_put(LED_G, 1); gpio_put(LED_B, 0); // Amarelo
    } else {
        gpio_put(LED_R, 1); gpio_put(LED_G, 0); gpio_put(LED_B, 0); // Vermelho
    }
}

// === Core 1 ===
void core1_main() {
    gpio_init(LED_R); gpio_init(LED_G); gpio_init(LED_B); gpio_init(BUZZER_PIN);
    gpio_set_dir(LED_R, GPIO_OUT); gpio_set_dir(LED_G, GPIO_OUT);
    gpio_set_dir(LED_B, GPIO_OUT); gpio_set_dir(BUZZER_PIN, GPIO_OUT);

    while (1) {
        if (multicore_fifo_rvalid()) {
            uint16_t estado_recebido = multicore_fifo_pop_blocking();
            ajustar_rgb(estado_recebido);

            if (estado_recebido > LIMIAR_CRITICO) {
                gpio_put(BUZZER_PIN, 1);
                sleep_ms(500);
                gpio_put(BUZZER_PIN, 0);
            }
        }
        sleep_ms(100); // reduz carga
    }
}

// === Main ===
int main() {
    stdio_init_all();
    multicore_launch_core1(core1_main);
    core0_main(); // roda no core 0
}
