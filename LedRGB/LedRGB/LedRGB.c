#include <stdio.h>
#include "pico/stdlib.h"

#define LED_R_PIN 13
#define LED_G_PIN 11
#define LED_B_PIN 12

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


    while (true) {

        gpio_put(LED_R_PIN, 1);   /*Coloca o pino LED_R_PIN como red*/
        gpio_put(LED_G_PIN, 0); /*Coloca o pino LED_G_PIN como green*/
        gpio_put(LED_B_PIN, 0);  /*Coloca o pino LED_B_PIN como blue*/
        printf("LED VERMELHO LIGADO\n");
        sleep_ms(1000);

        gpio_put(LED_R_PIN, 0);   /*Coloca o pino LED_R_PIN como red*/
        gpio_put(LED_G_PIN, 1); /*Coloca o pino LED_G_PIN como green*/
        gpio_put(LED_B_PIN, 0);  /*Coloca o pino LED_B_PIN como blue*/
        printf("LED VERDE LIGADO\n");
        sleep_ms(1000);

        gpio_put(LED_R_PIN, 0);   /*Coloca o pino LED_R_PIN como red*/
        gpio_put(LED_G_PIN, 0); /*Coloca o pino LED_G_PIN como green*/
        gpio_put(LED_B_PIN, 1);  /*Coloca o pino LED_B_PIN como blue*/
        printf("LED AZUL LIGADO\n");
        sleep_ms(1000);

        //sleep_ms(1000);
    }
}
