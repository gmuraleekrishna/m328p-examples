avr-gcc -v -g -Os -Wall -mcall-prologues -mmcu=atmega328p  -c -o $1.o $1 && \
avr-gcc -v -g -Os -Wall -mcall-prologues -mmcu=atmega328p $1.o -o $1.obj && \
avr-objcopy -v -R .eeprom -O ihex $1.obj $1.hex && \
avrdude  -p m328p -c avrispv2 -P $2 -U flash:w:$1.hex -U hfuse:w:0xD1:m -U efuse:w:0xFF:m  -U lfuse:w:0xFF:m

if [ "$3" == true ]; then
   minicom -o -8 -D $4 -b 9600 --color=on -l -H
fi


