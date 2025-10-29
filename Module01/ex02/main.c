#include <avr/io.h>

int main(void) {
    // 1. Configurer PB1 (OC1A) en sortie
    DDRB |= (1 << PB1);

    // 2. Configurer le Timer1 en mode Fast PWM (mode 14 : ICR1 comme TOP)
    TCCR1A = (1 << COM1A1) | (1 << WGM11);
    // COM1A1 = 1 → Clear OC1A on compare match, set at BOTTOM (mode non-inversé)
    // WGM11 = 1 → partie de la configuration Fast PWM (mode 14)
    
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS12) | (1 << CS10);
    // WGM13 + WGM12 = 1 pour Fast PWM avec ICR1 comme TOP
    // CS12 + CS10 = 1 → Prescaler = 1024

    // 3. Configurer la période : 1 Hz
    // f_timer = 16 MHz / 1024 = 15625 Hz
    // TOP = f_timer / f_pwm - 1 = 15625 / 1 - 1 = 15624
    ICR1 = 15624;

    // 4. Configurer le rapport cyclique : 10%
    // Duty = 0.1 * (ICR1 + 1) = 1562 environ
    OCR1A = 1562;

    // 5. Boucle infinie vide (PWM matériel)
    while (1) {
        // Rien à faire, le Timer1 gère la LED automatiquement
    }

    return 0;
}
