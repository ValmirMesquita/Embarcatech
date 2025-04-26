#include <stdio.h>
#include "pico/stdlib.h"

#define TAM_FILA 8
int fila[TAM_FILA];
int inicio = 0;
int fim = 0;
int quantidade = 0;

bool inserir(int valor) {
    if (quantidade == TAM_FILA) return false;
    fila[fim] = valor;
    fim = (fim + 1) % TAM_FILA;
    quantidade++;
    return true;
}

bool remover(int *valor) {
    if (quantidade == 0) return false;
    *valor = fila[inicio];
    inicio = (inicio + 1) % TAM_FILA;
    quantidade--;
    return true;
}

int main() {
    stdio_init_all();
    sleep_ms(2000);
    int contador = 0;
    while (true) {
        if (inserir(contador)) {
            printf("Produzido: %d\n", contador);
            contador++;
        } else {
            printf("Fila cheia! Não foi possível produzir.\n");
        }
        sleep_ms(1000);
        int valor;
        if (remover(&valor)) {
            printf("Consumido: %d\n", valor);
            printf("Inicio: %d\n", inicio);
            printf("Fim: %d\n", fim);
            printf("-----------------------------------\n");
            printf("br");
            
        } else {
            printf("Fila vazia! Nada para consumir.\n");
        }
        sleep_ms(100);
    }
}
