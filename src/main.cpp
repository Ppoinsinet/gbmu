#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include <cartridge.h>
#include <cpu.h>
#include <instructions.h>
#include <ui.h>
#include <interrupts.h>
#include <io.h>
#include <timer.h>

#include <pthread.h>
#include <unistd.h>
#include <bus.h>
#include <algorithm>
#include <dbg.h>
#include <iostream>
#include <vector>
#include <map>
#include <array>

unsigned long count = 0;

void execute_loop() {
    while (1) {

        // if (enable_print)
        //         usleep(1000);

        if (!cpu.halt) {

            count++;

            std::map<int, int> myVector{ {1, 2} };

            if (std::find(std::begin(breakpoints), std::end(breakpoints), read_reg(REG_PC)) != std::end(breakpoints)) {
                enable_print = 1;
            }

            INSTRUCTION instr = getInstruction(read_mem(read_reg(REG_PC)));
            emu_cycle(1);
            instruction_process(instr);
            // exit(1);
        

        } else {
            // printf("halted\n");
            // exit(2);
            emu_cycle(1);

            if (cpu.int_flags)
                cpu.halt = 0;

        }

        if (cpu.IME) {
            enable_print = 1;
            printf("Handling interrupts\n");
            // exit(1);
            cpu_handle_interrupts();
            cpu.enabling_IME = 0;
        }

        if (cpu.enabling_IME) {
            cpu.IME = 1;
        }

        if (cpu.increment_ly) {
            increment_ly();
            cpu.increment_ly = 0;
        }
    }
}

void *execute_program(void *arg) {
    FILE *file = (FILE *)arg;
    printf("Title : %s\n", cart.title);
    printf("File size : %ld\n", cart.size);
    printf("CGB Flag : %x\n", cart.CGB);
    printf("License : %s\n", cart.licensee);
    printf("Cartridge type : %s\n", cart.type);
    fclose(file);
    
    execute_loop();
    return NULL;
}

int main(int ac, char **av) {
    if (ac != 2) {
        printf("No files specified\n");
        return 1;
    }
    FILE *file = fopen(av[1], "rb");
    if (!file) {
        printf("Could not open cartridge\n");
        return 2;
    }

    fseek(file, 0L, SEEK_END);

    if (!(cart.size = ftell(file))) {
        printf("Could not define file size\n");
        return 2;
    }
    fseek(file, 0L, SEEK_SET);
    if (!(cart.data = (char *)malloc(sizeof(char) * cart.size))) {
        printf("Could not get file buffer\n");
        return 3;
    }
    unsigned long rd;
    if (!(rd = fread(cart.data, 1, cart.size, file))) {
        printf("Could not read file\n");
        return 4;
    }

    cart.title = &cart.data[CARTRIDGE_TITLE];
    cart.CGB = cart.data[CARTRIDGE_CGB_FLAG];
    cart.licensee = Licensee[cart.data[0x14B]];
    cart.type = cartridge_type[cart.data[0x147]];

    for (unsigned long i = 0; i < rd; i++) {
        ram.data[i] = cart.data[i];
    }

    pthread_t working_thread;

    init_cpu();
    timer.set_div(0xABCC);
    if (pthread_create(&working_thread, NULL, execute_program, NULL)) {
        printf("Could not start display thread\n");
        exit(2);
    }

    start_display(NULL);


    return 0;
}