WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wstrict-prototypes

CFLAGS := -Os -DF_CPU=16000000UL -mmcu=atmega328p $(WARNINGS)

upload: program.hex
	avrdude -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:program.hex

program.lst: program.elf
	avr-objdump -D -S program.elf > program.lst

program.hex: program.elf
	avr-objcopy -O ihex -R .eeprom program.elf program.hex

program.elf: main.c uart.c
	avr-gcc $(CFLAGS)  $^ -o $@

clean:
	rm -f *.elf *.hex *.o
