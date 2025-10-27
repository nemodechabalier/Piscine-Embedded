CC = avr-gcc

F_CPU = 16000000UL
BAUD = 115200

PORT = /dev/ttyUSB0
MCU=atmega328p

HEX_TARGET = main.hex

BIN_TARGET = main.bin
SRC_FILES = main.c

CC_FLAGS = -Wall -Wextra -Werror -O1 -DF_CPU=$(F_CPU) -mmcu=$(MCU)

AVR_OBJCOPY_FLAGS = -O ihex $(BIN_TARGET) $(HEX_TARGET)

AVR_DUDE_FLAGS = -c arduino -p m328p -b $(BAUD) -P $(PORT) -U flash:w:$(HEX_TARGET)

all: hex flash

flash:  hex
	avrdude $(AVR_DUDE_FLAGS)
    
hex: $(HEX_TARGET)

$(HEX_TARGET): $(BIN_TARGET)
	avr-objcopy $(AVR_OBJCOPY_FLAGS) 

$(BIN_TARGET):
	$(CC) $(CC_FLAGS) -o $(BIN_TARGET) $(SRC_FILES)

clean: 
	rm -rf $(HEX_TARGET) $(BIN_TARGET)

re: clean all flash

.PHONY : all hex re flash clean