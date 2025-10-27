#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

int main(void) {
    // Configuration
    DDRD &= ~(1 << PD2); // PD2 en entrée (bouton)
    PORTD |= (1 << PD2); // Pull-up activé sur PD2
    DDRB |= (1 << PB0);  // PB0 en sortie (LED)
    
    while(1) {
        if (!(PIND & (1 << PD2))) { // Bouton pressé (0 avec pull-up)
            PORTB |= (1 << PB0);   // LED ON 
        } else { // Bouton relâché (1 avec pull-up)
            PORTB &= ~(1 << PB0);  // LED OFF
        }
    }
}