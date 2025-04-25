#include <stdio.h>                         // Biblioteca padrão para entrada e saída (ex: printf)
#include "pico/stdlib.h"                  // Biblioteca do SDK do Raspberry Pi Pico (GPIO, sleep, etc.)
#include "hardware/pwm.h"                 // Biblioteca para controle de hardware PWM no Pico

// Define constantes e variáveis
#define LED 12                             // Define o pino 12 como LED controlado por PWM
#define LED2 13                            // Define o pino 13 como segundo LED (não usado no PWM)
const uint16_t Periodo = 2000;             // Valor máximo da contagem do PWM (define resolução)
const uint DivisorPWM = 16;                // Divisor da frequência do clock do PWM (define frequência final)
const uint16_t LED_STEP = 100;             // Passo de incremento/decremento do brilho
uint16_t LED_LEVEL = 100;                  // Brilho inicial do LED (nível PWM)

// Função de configuração do PWM
void setup_pwm()
{
  gpio_set_function(LED, GPIO_FUNC_PWM);     // Configura o pino 12 como função PWM
  uint slice = pwm_gpio_to_slice_num(LED);   // Obtém o número do "slice" PWM do pino 12
  printf("Slice LED: %d\n", slice);          // Imprime o número do slice do LED 1 para depuração

  uint slice2 = pwm_gpio_to_slice_num(LED2); // Obtém o slice do pino 13 (só imprime, não usa)
  printf("Slice LED: %d\n", slice2);         // Imprime o número do slice do LED 2

  pwm_set_clkdiv(slice, DivisorPWM);         // Define o divisor do clock do PWM (frequência mais baixa)
  pwm_set_wrap(slice, Periodo);              // Define o valor de contagem máxima (resolução do PWM)
  pwm_set_gpio_level(LED, LED_LEVEL);        // Define o nível inicial do PWM (brilho do LED)
  pwm_set_enabled(slice, true);              // Ativa o PWM para o slice correspondente
}

// Função principal
int main()
{
  uint up_down = 1;                          // Variável de controle para subir (1) ou descer (0) o brilho
  stdio_init_all();                          // Inicializa a comunicação serial (necessária para printf)
  setup_pwm();                               // Chama a função para configurar o PWM

  while (true)                               // Loop infinito
  {
    pwm_set_gpio_level(LED, LED_LEVEL);      // Atualiza o nível de brilho no pino PWM
    sleep_ms(100);                           // Espera 100 milissegundos

    if(up_down)                              // Se o brilho estiver aumentando
    {
      sleep_ms(100);                         // Espera mais um pouco
      printf("%d\n", LED_LEVEL);             // Imprime o nível atual do brilho
      LED_LEVEL = LED_LEVEL + LED_STEP;      // Aumenta o nível do brilho

      if(LED_LEVEL >= Periodo)               // Se o nível chegou no valor máximo
        up_down = 0;                         // Começa a diminuir o brilho
    }
    else                                     // Se o brilho estiver diminuindo
    {
      sleep_ms(100);                         // Espera mais um pouco
      printf("%d\n", LED_LEVEL);             // Imprime o nível atual do brilho
      LED_LEVEL = LED_LEVEL - LED_STEP;      // Diminui o nível do brilho

      if (LED_LEVEL <= LED_STEP)             // Se o nível chegou próximo do mínimo
        up_down = 1;                         // Começa a aumentar o brilho novamente
    }
  }
}