#ifndef MEM_H
#define MEM_H

#include <stdint.h>

uint8_t mem[0x1000];

void mem_load_rom();
uint8_t mem_read_byte(uint16_t addr);

void print_mem();

#endif
