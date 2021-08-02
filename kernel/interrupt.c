/*  Author: Joseph Shimel
    Date:   7/27/21 */
    
#include "./interrupt.h"
#include "../devices/io.h"
#include "../lib/stdio.h"

isr_t interrupt_handlers[256];

char *exception_messages[] = {
    "Division By Zero",
    "Debug",
    "Non Maskable Interrupt",
    "Breakpoint",
    "Into Detected Overflow",
    "Out of Bounds",
    "Invalid Opcode",
    "No Coprocessor",

    "Double Fault",
    "Coprocessor Segment Overrun",
    "Bad TSS",
    "Segment Not Present",
    "Stack Fault",
    "General Protection Fault",
    "Page Fault",
    "Unknown Interrupt",

    "Coprocessor Fault",
    "Alignment Check",
    "Machine Check",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",

    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved",
    "Reserved"
};

/*  Called after an ISR interrupt is raised. Not much to do, just print an
    error message */
void Interrupt_ISRHandler(registers_t r) {
    if(r.int_no < 32) {
        printf("EXCEPTION: ");
        printf(exception_messages[r.int_no]);
        for(;;);
    }
    if(interrupt_handlers[r.int_no] != NULL) {
         isr_t handler = interrupt_handlers[r.int_no];
         handler(&r);
    }
}

/*  Called from interrupt_table.asm after an IRQ is raised. Checks the 
    dispatch table of function pointers, calls the function with the registered 
    IRQ number. 
    
    @param r    Pointer to a register state struct */
void Interrupt_IRQHandler(registers_t* r) {
	// Need to send EOI to PICs or they will not send another interrupt
	if (r->int_no >= 40) {
		IO_OutByte(0xA0, 0x20); // slave
	}
	IO_OutByte(0x20, 0x20); // master

    if(interrupt_handlers[r->int_no] != NULL) {
        isr_t handler = interrupt_handlers[r->int_no];
        handler(r);
    }
}

/*  Adds a function pointer to the interrupt handlers array, which is used to
    dispatch code when an IRQ is raised
    
    @param n        IRQ id number
    @param handler  Function pointer to an IRQ handler */
void Interrupt_RegisterIRQHandler(uint8_t n, isr_t handler) {
	interrupt_handlers[n] = handler;
}