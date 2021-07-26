C_SOURCES = $(wildcard boot/*.c devices/*.c lib/*.c)
HEADERS = $(wildcard boot/*.h devices/*.h lib/*.h)
OBJ = ${C_SOURCES:.c=.o}

os-image.bin: ${OBJ}
	i686-elf-gcc -T boot/linker.ld -o os-image.bin -ffreestanding -O2 -nostdlib boot/boot.o ${OBJ} -lgcc

boot/kernel.o: ${OBJ}
	i686-elf-gcc -c boot/kernel.c -o boot/kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra

boot/boot.o: boot/boot.asm
	nasm -felf32 boot/boot.asm -o boot/boot.o

%.o: %.c ${HEADERS}
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -O2 -Wall -Wextra

run: os-image.bin
	qemu-system-i386 -kernel os-image.bin
	make clean

clean:
	rm -rf ${OBJ} os-image.bin

git-commit:
	make clean
	git add .
	git commit -m "$(msg)"

git-push:
	git push origin main