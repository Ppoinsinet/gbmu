#include <timer.h>
#include <interrupts.h>
#include <cpu.h>


struct {
    unsigned short div;
    unsigned char tima;
    unsigned char tma;
    unsigned char tac;
} timer = {.div = 0, .tma = 0, .tima = 0, .tac = 0};

unsigned char timer_read(unsigned short addr) {
    switch (addr)
    {
    case 0xFF04:
        return timer.div >> 8;
    case 0xFF05:
        return timer.tima;
    case 0xFF06:
        return timer.tma;
    case 0xFF07:
        return timer.tac;
    default:
        break;
    }
    ERROR("Error reading timer");
}

void timer_write(unsigned short addr, unsigned char val) {
    switch (addr)
    {
    case 0xFF04:
        timer.div = 0;
        break;
    case 0xFF05:
        timer.tima = val;
        break;
    case 0xFF06:
        timer.tma = val;
        break;
    case 0xFF07:
        timer.tac = val;
        break;
    
    default:
        ERROR("Error writing timer");
        break;
    }
}

void timer_tick() {
    unsigned short prev_div = timer.div++;

    char timer_update = 0;
    
    switch(timer.tac & (0b11)) {
        case 0b00:
            timer_update = (prev_div & (1 << 9)) && (!(timer.div & (1 << 9)));
            break;
        case 0b01:
            timer_update = (prev_div & (1 << 3)) && (!(timer.div & (1 << 3)));
            break;
        case 0b10:
            timer_update = (prev_div & (1 << 5)) && (!(timer.div & (1 << 5)));
            break;
        case 0b11:
            timer_update = (prev_div & (1 << 7)) && (!(timer.div & (1 << 7)));
            break;
    }

    if (timer_update && timer.tac & (1 << 2)) {
        timer.tima++;

        if (timer.tima == 0xFF) {
            timer.tima = timer.tma;

            request_interrupt(INT_TIMER);
        }
    }
}