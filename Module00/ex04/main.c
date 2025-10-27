#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
# define F_CPU 16000000UL
#endif
void display (uint8_t value) {
    PORTB &= ~((1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4));
    if (value & (1 << 0)) PORTB |= (1 << PB0);
    if (value & (1 << 1)) PORTB |= (1 << PB1); 
    if (value & (1 << 2)) PORTB |= (1 << PB2);
    if (value & (1 << 3)) PORTB |= (1 << PB4);
}

int main(void) {
    // Configuration
    DDRD &= ~(1 << PD2) | ~(1 << PD4); // PD2 PD4 en entrée (bouton)
    PORTD |= (1 << PD2) | (1 << PD4); // Pull-up activé sur PD2 PD4
    DDRB |= (1 << PB0) | (1 << PB1) | (1 << PB2) | (1 << PB4);  // PB0 en sortie (LED)
    uint8_t value = 0;

    while(1) {
        if (!(PIND & (1 << PD2))) { // Bouton pressé (0 avec pull-up)
            value = (value + 1) & 0xF;
            while(!(PIND & (1 << PD2)))
                _delay_ms(10);
        }
        _delay_ms(10);
        if (!(PIND & (1 << PD4))) { // Bouton pressé (0 avec pull-up)
            value = (value - 1) & 0xF;
            while(!(PIND & (1 << PD4)))
                _delay_ms(10);
        }
        _delay_ms(10);
        display (value);
    }
}