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

void uart_printstr(const char* str){
    for(int i = 0; str[i]; i++)
        uart_tx(str[i]);
}

void timer_init() {
    TCCR1A = 0;

    TCCR1B =  (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
    // WGM13 + WGM12 = 1 pour Fast PWM avec ICR1 comme TOP
    // CS12 + CS10 = 1 → Prescaler = 1024

    // 3. Configurer la période : 0.5 Hz
    // f_timer = 16 MHz / 1024 = 15625 Hz
    ICR1 = 15625 * 2;
    
    TIMSK1 = (1 << OCIE1A);

    sei();
}

ISR(TIMER1_COMPA_vect) {
    uart_printstr("Hello World!\r\n");
}

int main(void) {
    timer_init();
    uart_init();
    while (1) 
    {
    }
}