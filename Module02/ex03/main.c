// ...existing code...
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

void uart_init(){
    UCSR0A = (1 << U2X0);                 // Double speed
    UBRR0L = (uint8_t)UBRR_VALUE;         // Baud low
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);  // Baud high
    UCSR0B = (1 << TXEN0) | (1 << RXEN0) | (1 << RXCIE0); // TX, RX, RX interrupt
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);               // 8N1
}

static inline void uart_tx(char c){
    while (!(UCSR0A & (1 << UDRE0)));     // Wait TX buffer empty
    UDR0 = c;
}

static inline char convert_char(char c) {
    if (c >= 'a' && c <= 'z') return c - 32;
    if (c >= 'A' && c <= 'Z') return c + 32;
    return c;
}

// REMOVE the TIMER1 ISR. Use the UART RX ISR:
ISR(USART_RX_vect) {
    char c = UDR0;            // Read received byte (clears RXC)
    c = convert_char(c);
    if (c == '\r' || c == '\n') {
        uart_tx('\r');
        uart_tx('\n');
    } else {
        uart_tx(c);
    }
}

int main(void) {
    uart_init();
    sei();                    // Enable global interrupts

    while (1) {
        // Empty loop. All work is done in ISR.
    }
}