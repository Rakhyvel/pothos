; Author: Joseph Shimel
; Date: 7/25/21

; Define some constants
MBALIGN  equ  1 << 0            ; align loaded modules on page boundaries
MEMINFO  equ  1 << 1            ; provide memory map
FLAGS    equ  MBALIGN | MEMINFO ; this is the Multiboot 'flag' field
MAGIC    equ  0x1BADB002        ; 'magic number' lets bootloader find the header
CHECKSUM equ -(MAGIC + FLAGS)   ; checksum of above, to prove we are multiboot

; Multiboot header, defines 
section .multiboot
align 4
    dd MAGIC
    dd FLAGS
    dd CHECKSUM


; Allocate 16 kiB of uninitialized data for the stack
section .bss
align 16
stack_bottom:
resb 16384  ; declare 16 kiB of uninitialized data
stack_top:


; Multiboot will begin the kernel at the _start label
section .text
global _start:function (_start.end - _start)    ; declare _start as a function symbol
_start:
    mov esp, stack_top  ; Set the stack pointer to the top of the stack

    ; TODO:
    ; - Set up GDT
    ; - Set up paging

    extern main
    call main

    ; kernel has returned, halt system
    cli
.hang:   
    hlt
    jmp .hang
.end: