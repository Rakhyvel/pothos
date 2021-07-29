#include "./idt.h"
#include "../devices/io.h"

/*
    Registers entries in the IDT for ISRs, IRQs, and does something with PICs
    though I'm not sure */
void IDT_Init() {
    IDT_SetGate(0, (uint32_t)isr0);
    IDT_SetGate(1, (uint32_t)isr1);
    IDT_SetGate(2, (uint32_t)isr2);
    IDT_SetGate(3, (uint32_t)isr3);
    IDT_SetGate(4, (uint32_t)isr4);
    IDT_SetGate(5, (uint32_t)isr5);
    IDT_SetGate(6, (uint32_t)isr6);
    IDT_SetGate(7, (uint32_t)isr7);
    IDT_SetGate(8, (uint32_t)isr8);
    IDT_SetGate(9, (uint32_t)isr9);
    IDT_SetGate(10, (uint32_t)isr10);
    IDT_SetGate(11, (uint32_t)isr11);
    IDT_SetGate(12, (uint32_t)isr12);
    IDT_SetGate(13, (uint32_t)isr13);
    IDT_SetGate(14, (uint32_t)isr14);
    IDT_SetGate(15, (uint32_t)isr15);
    IDT_SetGate(16, (uint32_t)isr16);
    IDT_SetGate(17, (uint32_t)isr17);
    IDT_SetGate(18, (uint32_t)isr18);
    IDT_SetGate(19, (uint32_t)isr19);
    IDT_SetGate(20, (uint32_t)isr20);
    IDT_SetGate(21, (uint32_t)isr21);
    IDT_SetGate(22, (uint32_t)isr22);
    IDT_SetGate(23, (uint32_t)isr23);
    IDT_SetGate(24, (uint32_t)isr24);
    IDT_SetGate(25, (uint32_t)isr25);
    IDT_SetGate(26, (uint32_t)isr26);
    IDT_SetGate(27, (uint32_t)isr27);
    IDT_SetGate(28, (uint32_t)isr28);
    IDT_SetGate(29, (uint32_t)isr29);
    IDT_SetGate(30, (uint32_t)isr30);
    IDT_SetGate(31, (uint32_t)isr31);

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
    IDT_SetGate(32, (uint32_t)irq0);
    IDT_SetGate(33, (uint32_t)irq1);
    IDT_SetGate(34, (uint32_t)irq2);
    IDT_SetGate(35, (uint32_t)irq3);
    IDT_SetGate(36, (uint32_t)irq4);
    IDT_SetGate(37, (uint32_t)irq5);
    IDT_SetGate(38, (uint32_t)irq6);
    IDT_SetGate(39, (uint32_t)irq7);
    IDT_SetGate(40, (uint32_t)irq8);
    IDT_SetGate(41, (uint32_t)irq9);
    IDT_SetGate(42, (uint32_t)irq10);
    IDT_SetGate(43, (uint32_t)irq11);
    IDT_SetGate(44, (uint32_t)irq12);
    IDT_SetGate(45, (uint32_t)irq13);
    IDT_SetGate(46, (uint32_t)irq14);
    IDT_SetGate(47, (uint32_t)irq15);

    IDT_Load();
}

/*
    Called to add a new row to the IDT along with the special flags needed. */
void IDT_SetGate(uint32_t n, uint32_t handler) {
    idt[n].low_offset = handler & 0xFFFF;
	idt[n].sel = KERNEL_CS;
	idt[n].always0 = 0;
	idt[n].flags = 0x8E;
	idt[n].high_offset = (handler) >> 16 & 0xFFFF;
}

/*
    Called after IDT is created, loads the address IDT into the IDT register */
void IDT_Load() {
    idt_reg.base = (uint32_t) &idt;
	idt_reg.limit = IDT_ENTRIES * sizeof(idt_gate_t) - 1;
	// Always load &idt_reg
	__asm__ __volatile__("lidtl (%0)" : : "r" (&idt_reg));
    asm volatile("sti");
}