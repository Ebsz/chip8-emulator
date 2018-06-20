#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stdbool.h>

uint8_t mem[0x1000]; // Main memory

bool mem_init(char* path);
uint8_t mem_read_byte(uint16_t addr);
void mem_write_byte(uint16_t addr, uint8_t byte);

void print_mem();

#endif
