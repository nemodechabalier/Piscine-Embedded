#include <avr/io.h>

int main(void)
{
    // Configuration explicite
    DDRB |= (1 << DDB0);   // PB0 en sortie
    PORTB |= (1 << PB0);   // PB0 à HIGH
    
    while (1) {
        // Boucle infinie - LED reste allumée
    }
    return 0;
}