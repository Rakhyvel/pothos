/*  Author: Joseph Shimel
    Date:   7/27/21 */
    
#include "./pic.h"
#include "./io.h"

/*
 * PIC is very complex, for a better understanding, visist
 * http://www.brokenthorn.com/Resources/OSDevPic.html or some other materials that explain PIC, otherwise the following code is impossible to uderstand....
 * */
void PIC_Init() {
    // ICW1
    IO_OutByte(PIC1_COMMAND, ICW1);
    IO_OutByte(PIC2_COMMAND, ICW1);

    // ICW2, irq 0 to 7 is mapped to 0x20 to 0x27, irq 8 to F is mapped to 28 to 2F
    IO_OutByte(PIC1_DATA, 0x20);
    IO_OutByte(PIC2_DATA, 0x28);

    // ICW3, connect master pic with slave pic
    IO_OutByte(PIC1_DATA, 0x4);
    IO_OutByte(PIC2_DATA, 0x2);

    // ICW4, set x86 mode
    IO_OutByte(PIC1_DATA, 1);
    IO_OutByte(PIC2_DATA, 1);

    // clear the mask register
    IO_OutByte(PIC1_DATA, 0);
    IO_OutByte(PIC2_DATA, 0);
}

/*  Sends aknowledgement to the PIC 

    @param irq  IRQ of interrupt to acknowledge */
void PIC_IRQAcknowledge(uint8_t irq) {
    if(irq >= 0x28)
        IO_OutByte(PIC2, PIC_EOI);
    IO_OutByte(PIC1, PIC_EOI);
}
