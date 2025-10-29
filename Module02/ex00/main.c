#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

#ifndef BAUD
# define BAUD 115200
#endif

#define UBRR_VALUE (F_CPU/(8 * BAUD) - 1)

//UBRR0L = (uint8_t)UBRR_VALUE;
//UBRR0H = (uint8_t)(UBRR_VALUE >> 8);

void uart_init(){
    UCSR0A = (1 << U2X0);
    //Configurer UBRR0H et UBRR0L avec UBRR_VALUE
    UBRR0L = (uint8_t)UBRR_VALUE;
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    //Activer la transmission (TXEN0) dans UCSR0B
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    //Configurer le mode asynchrone 8N1 dans UCSR0C
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    //activer le double speed 
}

void uart_tx(char c){
    //Attendre que UDRE0 (USART Data Register Empty) soit à 1
        while (!(UCSR0A & (1 << UDRE0)));
        //Écrire c dans UDR0
        UDR0 = c;
}

int main(void) {
    uart_init();
    while (1) 
    {
        uart_tx('Z');
        _delay_ms(1000); // 1 Hz
    }
}