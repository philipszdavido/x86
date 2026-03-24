gcc -m32 -nostdlib -static -o program.elf program.c
objcopy -O binary program.elf program.bin


gcc -m32 -ffreestanding -c kernel.c
ld -T linker.ld -o kernel.elf kernel.o
objcopy -O binary kernel.elf kernel.bin