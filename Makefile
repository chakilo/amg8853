TARGET = amg8853
MCU = atmega64
F_CPU = 7372800
CFLAGS = -D F_CPU=$(F_CPU) -mmcu=$(MCU) -g -Os -funsigned-char -funsigned-bitfields -fpack-struct -fshort-enums -Wall

%.a: %.o
	avr-ar ru $(TARGET).a $< $@

%.o: %.c
	avr-gcc -c $(CFLAGS) $< -o $@

all: $(TARGET).a

clean:
	rm -f *.o
	rm -f *.a

.PHONY: all clean
