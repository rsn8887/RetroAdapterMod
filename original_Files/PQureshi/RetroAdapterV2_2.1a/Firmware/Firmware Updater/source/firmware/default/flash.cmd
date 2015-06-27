avrdude -p atmega168 -U lfuse:w:0xef:m -U hfuse:w:0xdf:m -U efuse:w:0xf8:m
avrdude -p atmega168 -B 1 -U flash:w:RAbootloader.hex
pause