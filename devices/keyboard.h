#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../kernel/interrupt.h"

void Keyboard_Handler(registers_t* r);
void Keyboard_Init();

#endif