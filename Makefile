C_SOURCES = $(wildcard boot/*.c devices/*.c lib/*.c threads/*.c kernel/*.c mem/*.c)
HEADERS = $(wildcard boot/*.h devices/*.h lib/*.h threads/*.h kernel/*.h mem/*.h)
OBJ = ${C_SOURCES:.c=.o} boot/boot.o kernel/interrupt_helper.o kernel/gdt_helper.o

os-image.bin:  ${OBJ}
	i686-elf-gcc -T boot/linker.ld -o os-image.bin -ffreestanding -nostdlib ${OBJ} -lgcc

boot/kernel.o: ${OBJ}
	i686-elf-gcc -c boot/kernel.c -o boot/kernel.o -std=gnu99 -ffreestanding -Wall -Wextra

%.o: %.asm
	nasm -felf32 $< -o $@

%.o: %.c ${HEADERS}
	i686-elf-gcc -c $< -o $@ -std=gnu99 -ffreestanding -Wall -Wextra

run: os-image.bin
	qemu-system-i386 -serial stdio -kernel os-image.bin
	make clean

# Unused until I figure out how to install iso burning tool
pothos.iso: os-image.bin
	cp myos.bin isodir/boot/myos.bin
	cp grub.cfg isodir/boot/grub/grub.cfg
	grub-mkrescue -o myos.iso isodir

clean:
	rm -rf boot/boot.o ${OBJ} os-image.bin

git-commit: clean
	git add .
	git commit -m "$(msg)"

git-push:
	git push origin main