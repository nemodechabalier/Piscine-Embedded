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


#define RED_GAIN   1.0
#define GREEN_GAIN 0.6
#define BLUE_GAIN  0.8

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

void uart_init(){
    UCSR0A = (1 << U2X0);
    //Configurer UBRR0H et UBRR0L avec UBRR_VALUE
    UBRR0L = (uint8_t)UBRR_VALUE;
    UBRR0H = (uint8_t)(UBRR_VALUE >> 8);
    //Activer la transmission (TXEN0) dans UCSR0B
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);
    //Configurer le mode asynchrone 8N1 dans UCSR0C
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
    //activer le double speed 
}

//#include <math.h>

//uint8_t gamma_correct(uint8_t val) {
//    float normalized = val / 255.0f;
//    float corrected = pow(normalized, 2.2); // gamma = 2.2
//    return (uint8_t)(corrected * 255.0f);
//}

//uint8_t apply_rgb_correction(uint8_t value, float gain) {
//    float corrected = value * gain;
//    if (corrected > 255.0f) corrected = 255.0f;
//    return (uint8_t)corrected;
//}

//void set_rgb(uint8_t r, uint8_t g, uint8_t b) {
//    OCR0B = apply_rgb_correction(r, RED_GAIN);
//    OCR0A = apply_rgb_correction(g, GREEN_GAIN);
//    OCR2B = apply_rgb_correction(b, BLUE_GAIN);
//}

void set_rgb(uint8_t r, uint8_t g, uint8_t b){
    OCR0A = g;
    OCR0B = r;
    OCR2B = b;
}

// Retourne 0..15 pour un chiffre hex valide, 0xFF sinon
uint8_t hex_char_to_val(char c) {
    if (c >= '0' && c <= '9') return c - '0';
    if (c >= 'A' && c <= 'F') return c - 'A' + 10;
    if (c >= 'a' && c <= 'f') return c - 'a' + 10;
    return 0xFF; // invalide
}

// Convertit deux hex en octet ; retourne 1 si ok, 0 si erreur
int hex_to_byte(char high, char low, uint8_t *out) {
    uint8_t hi = hex_char_to_val(high);
    uint8_t lo = hex_char_to_val(low);
    if (hi == 0xFF || lo == 0xFF) return 0; // caractère non hex
    *out = (hi << 4) | lo;
    return 1;
}

// parse_rgb vérifie le format et renvoie 1 si tout OK
int parse_rgb(const char *str, uint8_t *r, uint8_t *g, uint8_t *b) {
    if (!str) return 0;
    if (str[0] != '#') return 0;
    // on suppose que str contient au moins 7 caractères accessibles
    if (!hex_to_byte(str[1], str[2], r)) return 0;
    if (!hex_to_byte(str[3], str[4], g)) return 0;
    if (!hex_to_byte(str[5], str[6], b)) return 0;
    return 1;
}

char uart_rx() {
    while (!(UCSR0A & (1 << RXC0)));
    char c = UDR0;
    return c;
}

void uart_tx(char c){
    //Attendre que UDRE0 (USART Data Register Empty) soit à 1
    while (!(UCSR0A & (1 << UDRE0)));
    //Écrire c dans UDR0
    UDR0 = c;
}

void uart_printstr(const char* str){
    for(int i = 0; str[i]; i++)
        uart_tx(str[i]);
}

int main(void) {
    uart_init();
    init_rgb();
    uint8_t r, g, b = 0;
    char buffer[8];
    while (1) 
    {
        for (uint8_t i = 0; i < 7; i++){
            buffer[i] = uart_rx();
            uart_tx(buffer[i]);
        }
        buffer[7] = '\0';
        if (parse_rgb(buffer,&r,&g,&b)){
            set_rgb(r,g,b);
            uart_printstr("  Bon Format !!\r\n");
        }
        else
            uart_printstr("  Error Format !! Format = #RRGGBB\r\n");
    }
}