#ifndef TIMER_H
#define TIMER_H

void timer_write(unsigned short addr, unsigned char val);
unsigned char timer_read(unsigned short addr);

void timer_tick();

#endif