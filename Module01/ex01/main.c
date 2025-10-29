#include <avr/io.h>

int main(void) {
    TCCR1A = 0;
    TCCR1B = 0;
    // 1. Configuration de la pin PB1 en sortie
    DDRB |= (1 << PB1);    // PB1 en sortie pour OC1A (Output Compare 1A)
                            // DDB1 = bit de direction pour PB1
    
    // 2. Configuration du Timer1 en mode CTC (Clear Timer on Compare)
    TCCR1A = (1 << COM1A0); // COM1A0 = Toggle OC1A on Compare Match et est canal A pour PB1 = 6 TCCR1A sert a savoir quoi faire 
                            // Le timer inverse automatiquement PB1 à chaque match
                            // Pas de PWM classique, mais toggle automatique
    
    TCCR1B = (1 << WGM12) | (1 << CS12) | (1 << CS10); // TCCR1B sert a savoir comment compter
                            // WGM12 = Mode CTC (Clear Timer on Compare) quand les valeurs se rejoignent le timer est remis a 0
                            // CS12 + CS10 = Prescaler 1024
                            // Timer clock = 16MHz / 1024 = 15625 Hz
    
    // 3. Valeur de comparaison pour 1Hz
    OCR1A = 7812;           // OCR1A = Output Compare Register A
                            // 15625 Hz / 2 / 7813 ≈ 1 Hz
                            // /2 car toggle (change d'état) = demi-période
/*Fréquence CPU = 16 000 000 Hz (16 MHz)
Prescaler = 1024 (CS12=1, CS10=1)

Fréquence Timer = F_CPU / Prescaler
                = 16 000 000 / 1024
                = 15 625 Hz
OCR1A = 7812

Fréquence Toggle = 15 625 / (7812 + 1)
                 = 15 625 / 7 813
                 = 1.999 Hz ≈ 2 Hz
Fréquence Clignotement = 2 / 2 = 1 Hz*/
    // 4. Boucle infinie vide (obligatoire)
    while(1) {
        // VIDE ! Le timer gère tout automatiquement
    }
    
    return 0;
}