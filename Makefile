stk: 
	avr-gcc -mmcu=atmega8 -Wall -Os -o output.elf *.c
	avr-objcopy -O ihex -R .eeprom output.elf output.hex
	avrdude -c stk500v2 -p m8 -P $(AVR_DEVICE) -e -U flash:w:output.hex

usbasp:
	avr-gcc -mmcu=atmega8 -Wall -Os -o output.elf *.c
	avr-objcopy -O ihex -R .eeprom output.elf output.hex
	avrdude -c usbasp -p m8 -B 250 -e -U flash:w:output.hex


