#include "keyboard.h"
#include "./io.h"
#include "../kernel/interrupt.h"
#include "../lib/stdio.h"

void Keyboard_Handler(registers_t* r)
{
    int i, scancode;
    //get scancode with "timeout"
    for(i = 1000; i > 0; i++) {
        // Check if scan code is ready
        if((IO_InByte(0x64) & 1) == 0) continue;
        // Get the scan code
        scancode = IO_InByte(0x60);
        break;
    }
    if(i > 0) {
        if(scancode & 0x80) {
            // Key release
        }
        else {
            // Key down
            printf("Hello, world!\n");
        }
    }
}

void Keyboard_Init() {
    Interrupt_RegisterIRQHandler(IRQ1, &Keyboard_Handler);
}