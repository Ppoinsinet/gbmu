#include <timer.h>
#include <interrupts.h>
#include <cpu.h>
#include <iostream>

class Timer timer;

Timer::Timer()
: div(0), tma(0), tima(0), tac(0)
{}

void Timer::set_div(unsigned short val) {
    div = val;
}

unsigned char &Timer::read(unsigned short addr) {
    switch (addr)
    {
    case 0xFF04:
        return div_show;
    case 0xFF05:
        return tima;
    case 0xFF06:
        return tma;
    case 0xFF07:
        return tac;
    default:
        ERROR("Error reading timer");
    }
}

void Timer::write(unsigned short addr, unsigned char val) {
    switch (addr)
    {
    case 0xFF04:
        div = 0;
        div_show = div >> 8;
        break;
    case 0xFF05:
        tima = val;
        break;
    case 0xFF06:
        tma = val;
        break;
    case 0xFF07:
        tac = val;
        break;
    default:
        ERROR("Error writing timer");
    }
}

void Timer::tick() {
    unsigned short prev_div = div++;

    char timer_update = 0;
    
    switch(tac & (0b11)) {
        case 0b00:
            timer_update = (prev_div & (1 << 9)) && (!(div & (1 << 9)));
            break;
        case 0b01:
            timer_update = (prev_div & (1 << 3)) && (!(div & (1 << 3)));
            break;
        case 0b10:
            timer_update = (prev_div & (1 << 5)) && (!(div & (1 << 5)));
            break;
        case 0b11:
            timer_update = (prev_div & (1 << 7)) && (!(div & (1 << 7)));
            break;
    }

    if (timer_update && tac & (1 << 2)) {
        tima++;

        if (tima == 0xFF) {
            tima = tma;

            std::cerr << "Request interrupt";
            
            request_interrupt(INT_TIMER);
        }
    }
}