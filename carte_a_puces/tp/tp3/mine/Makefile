WARNINGS := -Wall -Wextra -pedantic -Wshadow -Wpointer-arith -Wcast-align \
            -Wwrite-strings -Wmissing-prototypes -Wmissing-declarations \
            -Wredundant-decls -Wnested-externs -Winline -Wno-long-long \
            -Wstrict-prototypes -Wno-array-bounds

CFLAGS := -Os -DF_CPU=16000000UL -mmcu=atmega328p $(WARNINGS)


overflow: program2.hex
	avrdude -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:$^

program2.hex: program2.elf
	avr-objcopy -O ihex $^ $@

program2.elf: overflow.c
	avr-gcc $(CFLAGS) -o $@ $<

compare: program1.hex
	avrdude -v -patmega328p -carduino -P/dev/ttyACM0 -b115200 -D -Uflash:w:$^

program1.hex: program1.elf
	avr-objcopy -O ihex $^ $@

program1.elf: compare.c
	avr-gcc $(CFLAGS) -o $@ $<

prepro.c: overflow.c
	avr-gcc $(CFLAGS) -E -o $@ $<

clean:
	rm -f *.elf prepro.c *.hex *.o
