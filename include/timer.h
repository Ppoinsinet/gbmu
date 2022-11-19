#ifndef TIMER_H
#define TIMER_H


class Timer
{
    private:
        unsigned short div;
        unsigned char div_show;
        unsigned char tima;
        unsigned char tma;
        unsigned char tac;
    public:
        void write(unsigned short addr, unsigned char val);
        unsigned char &read(unsigned short addr);
        void tick();
        
        void set_div(unsigned short val);
        Timer();

};

extern class Timer timer;

#endif