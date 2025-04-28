#include <stdio.h>                                      // Biblioteca padrão para entrada e saída, como printf
#include "pico/stdlib.h"                                // Biblioteca do SDK do Raspberry Pi Pico (GPIO, delays, etc.)
#include "hardware/pwm.h"                               // Biblioteca para configurar e usar PWM (modulação por largura de pulso)

#define DIV 100.0f
#define WRAP 12499
#define level (WRAP / 2)  // 50% duty cycle para um som "limpo"

#define DIV1 9.0f
#define WRAP1 12499
#define level1 (WRAP / 2)  // 50% duty cycle para um som "limpo"

const uint buzzer = 10;                                 // Define que o pino GPIO 10 será usado para o buzzer
const uint buzzer2 = 21;                                // Define que o pino GPIO 21 será usado para o buzzer

// const uint16_t WRAP = 23800;                            // Valor máximo da contagem do PWM (define a resolução/frequência)
// const float DIV = 16.0;                                 // Divisor do clock do PWM (define a frequência final)
// uint16_t level = 14930;                                 // Valor inicial do duty cycle (nível do sinal PWM)

//100 = 125_000_000 / (DIV * (WRAP + 1))
// WRAP = (125_000_000 / (100 * 100)) - 1 = 12499
void config_pwm() {
    gpio_set_function(buzzer, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(buzzer);
    pwm_set_clkdiv(slice, DIV);
    pwm_set_wrap(slice, WRAP);
    pwm_set_gpio_level(buzzer, level); 
    pwm_set_enabled(slice, true);

    sleep_ms(500);                    // Toca por 500 milissegundos (0.5 segundos)
    pwm_set_enabled(slice, false);    // Desliga o som
}

void config_pwm2() {
    gpio_set_function(buzzer2, GPIO_FUNC_PWM);
    uint slice = pwm_gpio_to_slice_num(buzzer);
    pwm_set_clkdiv(slice, DIV1);
    pwm_set_wrap(slice, WRAP1);
    pwm_set_gpio_level(buzzer2, level1); 
    pwm_set_enabled(slice, true);

    sleep_ms(500);                    // Toca por 500 milissegundos (0.5 segundos)
    pwm_set_enabled(slice, false);    // Desliga o som
}

// void config_pwm() {
//     gpio_set_function(buzzer, GPIO_FUNC_PWM);           // Configura o pino 10 como saída PWM
//     uint slice = pwm_gpio_to_slice_num(buzzer);         // Descobre qual slice controla o pino do buzzer
//     pwm_set_clkdiv(slice, DIV);                         // Define o divisor de clock (controla a frequência)
//     pwm_set_wrap(slice, WRAP);                          // Define o valor máximo do contador PWM (define a resolução)
//     pwm_set_gpio_level(buzzer, level);                  // Define o nível PWM (proporção do tempo em HIGH)
//     pwm_set_enabled(slice, true);                       // Ativa o PWM nesse slice (inicia o sinal)
// }

int main() {
    stdio_init_all();                                   // Inicializa o sistema de entrada/saída (útil para debug com printf)
    //config_pwm();                                       // Configura o PWM no pino do buzzer
    while (true) {
        config_pwm(); 
        sleep_ms(500); 
        config_pwm2(); 
        sleep_ms(500); 
        config_pwm(); 
        config_pwm2();
        config_pwm2();
        sleep_ms(100); 
        config_pwm(); 
        sleep_ms(500); 
        
    }                                     // Loop infinito vazio: mantém o PWM rodando indefinidamente
}