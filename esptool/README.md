open terminal 

Command:
<path\esptool_py\3.3.0/esptool.exe> --chip esp32s2 --port <PORT> --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 <path\arduino_build_409095/main.ino.bootloader.bin> 0x8000 <path\arduino_build_409095/main.ino.partitions.bin> 0xe000 <path\esp32\2.0.3/tools/partitions/boot_app0.bin> 0x10000 <path\arduino_build_409095/main.ino.bin> 


Example Command:
C:\Users\filip\AppData\Local\Arduino15\packages\esp32\tools\esptool_py\3.3.0/esptool.exe --chip esp32s2 --port COM5 --baud 921600 --before default_reset --after hard_reset write_flash -z --flash_mode dio --flash_freq 80m --flash_size 4MB 0x1000 C:\Users\filip\AppData\Local\Temp\arduino_build_409095/main.ino.bootloader.bin 0x8000 C:\Users\filip\AppData\Local\Temp\arduino_build_409095/main.ino.partitions.bin 0xe000 C:\Users\filip\AppData\Local\Arduino15\packages\esp32\hardware\esp32\2.0.3/tools/partitions/boot_app0.bin 0x10000 C:\Users\filip\AppData\Local\Temp\arduino_build_409095/main.ino.bin 
