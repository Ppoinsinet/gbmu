#include <dbg.h>
#include <bus.h>
#include <stdlib.h>

static char dbg_msg[1024] = {0};
static int msg_size = 0;

char enable_print = 0;

unsigned short breakpoints[] = {};

void dbg_update() {
    if (read_mem(0xFF02) == 0x81) {
        char c = read_mem(0xFF01);

        dbg_msg[msg_size++] = c;
        write_mem(0xFF02, 0);
    }
}

void dbg_print() {
    if (dbg_msg[0]) {
        printf("DEBUG : %s\n", dbg_msg);
        exit(3);
    }
}
