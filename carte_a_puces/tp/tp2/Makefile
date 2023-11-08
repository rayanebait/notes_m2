PROG=sleep


#reset: reset.hex
	#avrdude -patmega328p -carduino -P/dev/ttyACM0 -b115200 -Uflash:w:$^

upload: $(PROG).hex
	avrdude -patmega328p -carduino -P/dev/ttyACM0 -b115200 -Uflash:w:$^

%.hex: %.elf
	avr-objcopy -O ihex $^ $@

$(PROG).elf: $(PROG).o
	avr-ld $^ -o $@

%.o: %.asm
	avr-as $^ -o $@


clean:
	rm -f $(PROG).o $(PROG).hex $(PROG).elf
	rm -f reset.o reset.hex reset.elf
