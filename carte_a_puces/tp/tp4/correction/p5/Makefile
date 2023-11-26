WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wstrict-prototypes -Wno-array-bounds -Wno-int-to-pointer-cast \
            -Wno-unused-parameter

CFLAGS := -Os -DF_CPU=16000000UL -mmcu=atmega328p $(WARNINGS)

master: master.hex
	avrdude -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:$<

slave: slave.hex
	avrdude -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:$<

%.hex: %.elf
	avr-objcopy -O ihex -R .eeprom $< $@

master.elf: master.c uart.c ring_buffer.c proto.c
	avr-gcc $(CFLAGS)  $^ -o $@

slave.elf: slave.c uart.c ring_buffer.c proto.c
	avr-gcc $(CFLAGS)  $^ -o $@

clean:
	rm -f *.elf *.o *.hex

