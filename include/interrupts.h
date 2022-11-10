#ifndef INTERRUPTS_H
#define INTERRUPTS_H

typedef enum {
    INT_VBLANK = 1,
    INT_LCD_STAT = 2,
    INT_TIMER = 4,
    INT_SERIAL = 8,
    INT_JOYPAD = 16

} INT_TYPE;

void cpu_handle_interrupts();
char int_check(unsigned short addr, INT_TYPE type);
void int_handle(unsigned short addr);

#endif