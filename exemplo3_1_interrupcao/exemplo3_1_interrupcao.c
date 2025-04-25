#include <stdio.h>
#include "pico/stdlib.h"


#define LED_PIN 12
#define BUTTON_PIN 5

volatile bool flag_evento = false;

// retorno de chamada 
void gpio_callback(uint gpio, uint32_t events) {
    if (gpio == BUTTON_PIN && (events & GPIO_IRQ_EDGE_FALL)) {
        flag_evento = true;
    }
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    gpio_init(BUTTON_PIN);
    gpio_set_dir(BUTTON_PIN, GPIO_IN);
    gpio_pull_up(BUTTON_PIN);
    gpio_set_irq_enabled_with_callback(BUTTON_PIN, GPIO_IRQ_EDGE_FALL, true, &gpio_callback);
    
    while (true) {
        if (flag_evento) {
            printf("Interrupção detectada! Acionando LED...\n");
            gpio_put(LED_PIN, 1);
            sleep_ms(250);
            gpio_put(LED_PIN, 0);
            flag_evento = false;
        }
        tight_loop_contents();
    }
}
