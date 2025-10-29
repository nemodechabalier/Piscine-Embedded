#include <avr/io.h>
#include <util/delay.h>

#ifndef F_CPU
# define F_CPU 16000000UL
#endif

void update_pwm(uint8_t duty_percent) {
        // Activer PWM matériel
        TCCR1A |= (1 << COM1A1);
        OCR1A = (1562 * duty_percent);
}

int main(void) {
    DDRD &= ~(1 << PD2) | ~(1 << PD4);
    PORTD = (1 << PD2) | (1 << PD4); 
    // 1. Configurer PB1 (OC1A) en sortie //Output Compare timer 1 canal A
    DDRB = (1 << PB1);
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
    ICR1 = 15620;

    // 4. Configurer le rapport cyclique : 10%
    // Duty = 0.1 * (ICR1 + 1) = 1562 environ
    OCR1A = 1562;

    // 5. Boucle infinie vide (PWM matériel)
    uint8_t value = 1;
    while (1) {
       if (!(PIND & (1 << PD2))) { // Bouton pressé (0 avec pull-up)
            if (value != 10) 
            value ++;
            update_pwm(value);
            _delay_ms(10);
            while(!(PIND & (1 << PD2)))
                _delay_ms(10);
        }
        if (!(PIND & (1 << PD4))) { // Bouton pressé (0 avec pull-up)
            if (value != 1) 
                value -= 1;
            update_pwm(value);
            _delay_ms(10);
            while(!(PIND & (1 << PD4)))
                _delay_ms(10);
        }
    }
    return 0;
}
