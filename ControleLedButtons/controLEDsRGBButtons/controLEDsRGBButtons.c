#include <stdio.h>
#include "pico/stdlib.h"

#include "hardware/pwm.h"
#include "hardware/clocks.h"

#define LED_R_PIN 13
#define LED_G_PIN 11
#define LED_B_PIN 12
#define BTN_A_PIN 5
#define BTN_B_PIN 6

 // Configuração do pino do buzzer
 #define BUZZER_PIN 21

// Configuração da frequência do buzzer (em Hz)
#define BUZZER_FREQUENCY 8000


 // Definição de uma função para inicializar o PWM no pino do buzzer
 void pwm_init_buzzer(uint pin) {
    // Configurar o pino como saída de PWM
    gpio_set_function(pin, GPIO_FUNC_PWM);

    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o PWM com frequência desejada
    pwm_config config = pwm_get_default_config();
    pwm_config_set_clkdiv(&config, clock_get_hz(clk_sys) / (BUZZER_FREQUENCY * 4096)); // Divisor de clock
    pwm_init(slice_num, &config, true);

    // Iniciar o PWM no nível baixo
    pwm_set_gpio_level(pin, 0);
}

 // Definição de uma função para emitir um beep com duração especificada
 void beep(uint pin, uint duration_ms) {
    // Obter o slice do PWM associado ao pino
    uint slice_num = pwm_gpio_to_slice_num(pin);

    // Configurar o duty cycle para 50% (ativo)
    pwm_set_gpio_level(pin, 2048);

    // Temporização
    sleep_ms(duration_ms);

    // Desativar o sinal PWM (duty cycle 0)
    pwm_set_gpio_level(pin, 0);

    // Pausa entre os beeps
    sleep_ms(100); // Pausa de 100ms
}

/*--------- Prototipo das funçoes ---------- */
void set_leds(bool red, bool green, bool blue );
void set_pin();


/*--------- Função principal ---------- */
int main()
{
    stdio_init_all();

     // Inicializar o PWM no pino do buzzer
     pwm_init_buzzer(BUZZER_PIN);
     set_pin(); /*Chamada da set_pinos */

    while (true) {

        if (gpio_get(BTN_A_PIN) && gpio_get(BTN_B_PIN))
        {
            /*Chada da função void set_leds(bool red, bool green, bool blue ) 
            Ex: set_leds(1, 1, 1 ) Informando que os leds estaram em nivel logico alto
            com passagem dos parametros da função*/
            set_leds(1,1,1); 
            //beep(BUZZER_PIN, 500); // Bipe de 500ms 

        }
        else if (gpio_get(BTN_A_PIN))
        {
            set_leds(1,0,0);  /*Coloca o pino LED_B_PIN como blue*/
            beep(BUZZER_PIN, 500); // Bipe de 500ms

        }
        else if (gpio_get(BTN_B_PIN))
        {

            set_leds(0,1,0);  /*Coloca o pino LED_B_PIN como blue*/
            beep(BUZZER_PIN, 500); // Bipe de 500ms
        }
        else
        {
            // gpio_put(LED_R_PIN, 0);  /*Coloca o pino LED_R_PIN como red*/
            // gpio_put(LED_G_PIN, 0);  /*Coloca o pino LED_G_PIN como green*/
            // gpio_put(LED_B_PIN, 1);  /*Coloca o pino LED_B_PIN como blue*/

            set_leds(0,0,1);  /*Coloca o pino LED_B_PIN como blue*/
            beep(BUZZER_PIN, 500); // Bipe de 500ms
        }
        
        //printf("Hello, world!\n");
        sleep_ms(1000);
    }

    return 0;
}

/*Declaração das funcoes */

/*Defiçoões da pinagem de saidas dos pinos leds e botoes */
void set_pin()
{
    // Definiçoes de saida dos leds como saida
    gpio_init(LED_R_PIN);
    gpio_set_dir(LED_R_PIN, GPIO_OUT);
    
    gpio_init(LED_G_PIN);
    gpio_set_dir(LED_G_PIN, GPIO_OUT);
    
    gpio_init(LED_B_PIN);
    gpio_set_dir(LED_B_PIN, GPIO_OUT);
    
    // Definiçoes de saida dos botoes como entrada 
    gpio_init(BTN_A_PIN);
    gpio_set_dir(BTN_A_PIN, GPIO_IN);
    gpio_pull_up(BTN_A_PIN);

    gpio_init(BTN_B_PIN);
    gpio_set_dir(BTN_B_PIN, GPIO_IN);
    gpio_pull_up(BTN_B_PIN);
}


/*seta os pinos definidos (#define) conforme a variavel da função set_leds*/
void set_leds(bool red, bool green, bool blue )
{
    gpio_put(LED_R_PIN, red);   /*Coloca o pino LED_R_PIN como red*/
    gpio_put(LED_G_PIN, green); /*Coloca o pino LED_G_PIN como green*/
    gpio_put(LED_B_PIN, blue);  /*Coloca o pino LED_B_PIN como blue*/
}
