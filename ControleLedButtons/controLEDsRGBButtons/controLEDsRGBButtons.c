#include <stdio.h>
#include "pico/stdlib.h"

#define LED_R_PIN 13
#define LED_G_PIN 11
#define LED_B_PIN 12
#define BTN_A_PIN 5
#define BTN_B_PIN 6

/*seta os pinos definidos (#define) conforme a variavel da função set_leds*/
void set_leds(bool red, bool green, bool blue )
{
    gpio_put(LED_R_PIN, red);   /*Coloca o pino LED_R_PIN como red*/
    gpio_put(LED_G_PIN, green); /*Coloca o pino LED_G_PIN como green*/
    gpio_put(LED_B_PIN, blue);  /*Coloca o pino LED_B_PIN como blue*/
}

int main()
{
    stdio_init_all();

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

    while (true) {

        if (gpio_get(BTN_A_PIN) && gpio_get(BTN_B_PIN))
        {
            /*Chada da função void set_leds(bool red, bool green, bool blue ) 
            Ex: set_leds(1, 1, 1 ) Informando que os leds estaram em estado ativo
            com passagem dos parametros da função*/
            set_leds(1,1,1);  /*Coloca o pino LED_B_PIN como blue*/

        }
        else if (gpio_get(BTN_A_PIN))
        {
        
            set_leds(1,0,0);  /*Coloca o pino LED_B_PIN como blue*/
        }
        else if (gpio_get(BTN_B_PIN))
        {

            set_leds(0,1,0);  /*Coloca o pino LED_B_PIN como blue*/
        }
        else
        {
            // gpio_put(LED_R_PIN, 0);  /*Coloca o pino LED_R_PIN como red*/
            // gpio_put(LED_G_PIN, 0);  /*Coloca o pino LED_G_PIN como green*/
            // gpio_put(LED_B_PIN, 1);  /*Coloca o pino LED_B_PIN como blue*/

            set_leds(0,0,1);  /*Coloca o pino LED_B_PIN como blue*/
        }
        
        //printf("Hello, world!\n");
        sleep_ms(1000);
    }

    return 0;
}
