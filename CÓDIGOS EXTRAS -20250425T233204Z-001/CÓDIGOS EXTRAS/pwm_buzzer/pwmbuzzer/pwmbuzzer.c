#include <stdio.h>                                      // Biblioteca padrão para entrada e saída, como printf
#include "pico/stdlib.h"                                // Biblioteca do SDK do Raspberry Pi Pico (GPIO, delays, etc.)
#include "hardware/pwm.h"                               // Biblioteca para configurar e usar PWM (modulação por largura de pulso)

const uint buzzer = 10;                                 // Define que o pino GPIO 10 será usado para o buzzer
const uint16_t WRAP = 23800;                            // Valor máximo da contagem do PWM (define a resolução/frequência)
const float DIV = 16.0;                                 // Divisor do clock do PWM (define a frequência final)
uint16_t level = 14930;                                 // Valor inicial do duty cycle (nível do sinal PWM)

void config_pwm() {
    gpio_set_function(buzzer, GPIO_FUNC_PWM);           // Configura o pino 10 como saída PWM
    uint slice = pwm_gpio_to_slice_num(buzzer);         // Descobre qual slice controla o pino do buzzer
    pwm_set_clkdiv(slice, DIV);                         // Define o divisor de clock (controla a frequência)
    pwm_set_wrap(slice, WRAP);                          // Define o valor máximo do contador PWM (define a resolução)
    pwm_set_gpio_level(buzzer, level);                  // Define o nível PWM (proporção do tempo em HIGH)
    pwm_set_enabled(slice, true);                       // Ativa o PWM nesse slice (inicia o sinal)
}

int main() {
    stdio_init_all();                                   // Inicializa o sistema de entrada/saída (útil para debug com printf)
    config_pwm();                                       // Configura o PWM no pino do buzzer
    while (true) {}                                     // Loop infinito vazio: mantém o PWM rodando indefinidamente
}