#ifndef MEM_H
#define MEM_H

#include <stdint.h>
#include <stdbool.h>

// Initialize memory with program given path; returns true on success
bool mem_init(char* path);

// Read a single byte from memory at address addr
uint8_t mem_read_byte(uint16_t addr);

// Write a single byte to memory to addr
void mem_write_byte(uint16_t addr, uint8_t byte);

#endif
