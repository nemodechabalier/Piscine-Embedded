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


ISR(TIMER1_COMPA_vect) {
    static uint8_t nb = 0;
    PORTD &= ~((1 << PD5) | (1 << PD6) | (1 << PD3));
    if (nb == 0)
        PORTD |= (1 << PD5); // rouge ON
    else if (nb == 1)
        PORTD |= (1 << PD6); // vert ON
    else
        PORTD |= (1 << PD3); // bleu ON
    nb = (nb + 1) % 3;


}

int main(void) {
    DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD3); // sorties
    TCCR1B =  (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
    // WGM13 + WGM12 = 1 pour Fast PWM avec ICR1 comme TOP
    // CS12 + CS10 = 1 → Prescaler = 1024

    // 3. Configurer la période : 1 Hz
    // f_timer = 16 MHz / 1024 = 15625 Hz
    ICR1 = 15625;
    
    TIMSK1 = (1 << OCIE1A);

    sei();

    while (42) {}
}