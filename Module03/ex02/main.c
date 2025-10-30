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

}

void init_rgb(){
    DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD3); // sorties
    TCCR0A = (1 << COM0A1) | (1 << COM0B1) | (1 << WGM00) | (1 << WGM01);
    TCCR0B = (1 << CS01); // prescaler 8
    TCCR2A = (1 << COM2B1) | (1 << WGM20) | (1 << WGM21);
    TCCR2B = (1 << CS21);
}

void set_rgb(uint8_t r, uint8_t g, uint8_t b){
    OCR0A = g;
    OCR0B = r;
    OCR2B = b;
}

void wheel(uint8_t pos) {
    pos = 255 - pos;
    if (pos < 85) {
        set_rgb(255 - pos * 3, 0, pos * 3);
    } else if (pos < 170) {
        pos = pos - 85;
        set_rgb(0, pos * 3, 255 - pos * 3);
    } else {
        pos = pos - 170;
        set_rgb(pos * 3, 255 - pos * 3, 0);
    }
}

int main(void) {
    DDRD |= (1 << PD5) | (1 << PD6) | (1 << PD3); // sorties
    // WGM13 + WGM12 = 1 pour Fast PWM avec ICR1 comme TOP
    // CS12 + CS10 = 1 → Prescaler = 1024

    // 3. Configurer la période : 1 Hz
    // f_timer = 16 MHz / 1024 = 15625 Hz
    init_rgb();    

    while (42) {
        for (uint8_t pos = 0; ; pos++) {
            wheel(pos);
            _delay_ms(10);
        }
    }
}