#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stddef.h>
#include <stdint.h>

// Struct which aggregates many registers
typedef struct {
   uint32_t ds; // Data segment selector
   uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax; // Pushed by pusha.
   uint32_t int_no, err_code; // Interrupt number and error code (if applicable)
   uint32_t eip, cs, eflags, useresp, ss; // Pushed by the processor automatically
} registers_t;

typedef void (*isr_t)(registers_t*);

void Interrupt_RegisterIRQHandler(uint8_t n, isr_t handler);

#endif