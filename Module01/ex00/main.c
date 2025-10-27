#include <avr/io.h>

// Fonction de délai personnalisée (pas de _delay_ms)
void my_delay(void) {
    // Boucle pour créer un délai d'environ 500ms
    for (volatile uint32_t i = 0; i < 500000; i++) {
        // Boucle vide - crée le délai
    }
}

int main(void) {
    // Configuration de PB1 en sortie pour LED D2
    DDRB |= (1 << DDB1);   // DDB1 = bit 1 du registre de direction
    
    while (1) {
        // Toggle LED D2 avec opération XOR (pas de if/else)
        PORTB ^= (1 << PB1);   // ^= fait un XOR - inverse l'état
                               // Si LED OFF → ON, si LED ON → OFF
        my_delay();
    }
    
    // INTERDIT : pas de return dans la boucle infinie !
    // return 0;  ← Cette ligne ne doit jamais être atteinte
}