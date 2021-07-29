#include "./interrupt.h"
#include "../devices/io.h"
#include "../lib/stdio.h"

isr_t interrupt_handlers[256];

/*
    Registers entries in the IDT for ISRs, IRQs, and does something with PICs
    though I'm not sure */
void Interrupt_InstallISRs() {
    Interrupt_SetIDTGate(0, (uint32_t)isr0);
    Interrupt_SetIDTGate(1, (uint32_t)isr1);
    Interrupt_SetIDTGate(2, (uint32_t)isr2);
    Interrupt_SetIDTGate(3, (uint32_t)isr3);
    Interrupt_SetIDTGate(4, (uint32_t)isr4);
    Interrupt_SetIDTGate(5, (uint32_t)isr5);
    Interrupt_SetIDTGate(6, (uint32_t)isr6);
    Interrupt_SetIDTGate(7, (uint32_t)isr7);
    Interrupt_SetIDTGate(8, (uint32_t)isr8);
    Interrupt_SetIDTGate(9, (uint32_t)isr9);
    Interrupt_SetIDTGate(10, (uint32_t)isr10);
    Interrupt_SetIDTGate(11, (uint32_t)isr11);
    Interrupt_SetIDTGate(12, (uint32_t)isr12);
    Interrupt_SetIDTGate(13, (uint32_t)isr13);
    Interrupt_SetIDTGate(14, (uint32_t)isr14);
    Interrupt_SetIDTGate(15, (uint32_t)isr15);
    Interrupt_SetIDTGate(16, (uint32_t)isr16);
    Interrupt_SetIDTGate(17, (uint32_t)isr17);
    Interrupt_SetIDTGate(18, (uint32_t)isr18);
    Interrupt_SetIDTGate(19, (uint32_t)isr19);
    Interrupt_SetIDTGate(20, (uint32_t)isr20);
    Interrupt_SetIDTGate(21, (uint32_t)isr21);
    Interrupt_SetIDTGate(22, (uint32_t)isr22);
    Interrupt_SetIDTGate(23, (uint32_t)isr23);
    Interrupt_SetIDTGate(24, (uint32_t)isr24);
    Interrupt_SetIDTGate(25, (uint32_t)isr25);
    Interrupt_SetIDTGate(26, (uint32_t)isr26);
    Interrupt_SetIDTGate(27, (uint32_t)isr27);
    Interrupt_SetIDTGate(28, (uint32_t)isr28);
    Interrupt_SetIDTGate(29, (uint32_t)isr29);
    Interrupt_SetIDTGate(30, (uint32_t)isr30);
    Interrupt_SetIDTGate(31, (uint32_t)isr31);

    // Remap the PIC
    IO_OutByte(0x20, 0x11);
    IO_OutByte(0xA0, 0x11);
    IO_OutByte(0x21, 0x20);
    IO_OutByte(0xA1, 0x28);
    IO_OutByte(0x21, 0x04);
    IO_OutByte(0xA1, 0x02);
    IO_OutByte(0x21, 0x01);
    IO_OutByte(0xA1, 0x01);
    IO_OutByte(0x21, 0x0);
    IO_OutByte(0xA1, 0x0); 

    // Install the IRQs
    Interrupt_SetIDTGate(32, (uint32_t)irq0);
    Interrupt_SetIDTGate(33, (uint32_t)irq1);
    Interrupt_SetIDTGate(34, (uint32_t)irq2);
    Interrupt_SetIDTGate(35, (uint32_t)irq3);
    Interrupt_SetIDTGate(36, (uint32_t)irq4);
    Interrupt_SetIDTGate(37, (uint32_t)irq5);
    Interrupt_SetIDTGate(38, (uint32_t)irq6);
    Interrupt_SetIDTGate(39, (uint32_t)irq7);
    Interrupt_SetIDTGate(40, (uint32_t)irq8);
    Interrupt_SetIDTGate(41, (uint32_t)irq9);
    Interrupt_SetIDTGate(42, (uint32_t)irq10);
    Interrupt_SetIDTGate(43, (uint32_t)irq11);
    Interrupt_SetIDTGate(44, (uint32_t)irq12);
    Interrupt_SetIDTGate(45, (uint32_t)irq13);
    Interrupt_SetIDTGate(46, (uint32_t)irq14);
    Interrupt_SetIDTGate(47, (uint32_t)irq15);

    Interrupt_LoadIDT();
}

/*
    Called to add a new row to the IDT along with the special flags needed. */
void Interrupt_SetIDTGate(uint32_t n, uint32_t handler) {
    idt[n].low_offset = handler & 0xFFFF;
	idt[n].sel = KERNEL_CS;
	idt[n].always0 = 0;
	idt[n].flags = 0x8E;
	idt[n].high_offset = (handler) >> 16 & 0xFFFF;
}

/*
    Called after IDT is created, loads the address IDT into the IDT register */
void Interrupt_LoadIDT() {
    idt_reg.base = (uint32_t) &idt;
	idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
	// Always load &idt_reg
	__asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
    asm volatile("sti");
}

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

/*
    Called after an ISR interrupt is raised. Not much to do, just print an
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

/*
    Called from interrupt_table.asm after an IRQ is raised. Checks the 
    dispatch table of function pointers, calls the function with the registered 
    IRQ number. */
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

/*
    Adds a function pointer to the interrupt handlers array, which is used to
    dispatch code when an IRQ is raised */
void Interrupt_RegisterIRQHandler(uint8_t n, isr_t handler) {
	interrupt_handlers[n] = handler;
}