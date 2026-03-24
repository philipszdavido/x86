#  x86 emulator

```sh
gcc -m32 -nostdlib -static -o program.elf program.c
objcopy -O binary program.elf program.bin
```
