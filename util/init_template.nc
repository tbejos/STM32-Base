reset halt
flash write_image erase bin/main.bin 0x08000000
exit
