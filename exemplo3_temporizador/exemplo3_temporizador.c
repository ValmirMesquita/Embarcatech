#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/timer.h"
#define LED_PIN 12
#define BTN_A_PIN 5

volatile bool alternar_led = false;

bool estado_led = false;

int64_t temporizador_callback(alarm_id_t id, void *user_data) {
    alternar_led = true;
    return 1000;
}
int main() {
    stdio_init_all();
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);
    add_alarm_in_ms(1000, temporizador_callback, NULL, true);

     // Definiçoes de saida dos botoes como entrada 
     gpio_init(BTN_A_PIN);
     gpio_set_dir(BTN_A_PIN, GPIO_IN);
     gpio_pull_up(BTN_A_PIN);
 
    
    while (true) {
        if (alternar_led) {
            estado_led = !estado_led;
            gpio_put(LED_PIN, estado_led);
            printf("LED = %d\n", estado_led);
            
            sleep_ms(100);
            alternar_led = false;
        }else
        {
            if (gpio_get(BTN_A_PIN))
        {
            estado_led = !estado_led;
            gpio_put(LED_PIN, estado_led);
            printf("LED = %d\n", estado_led);
            printf("Botao precionado led acelerado");
            
            sleep_ms(2000);
            alternar_led = false;
        }
        }
        

       
        
        tight_loop_contents();
    }
}
