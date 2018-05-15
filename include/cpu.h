
#ifndef CPU_H
#define CPU_H

#include <stdint.h>

uint8_t GP_REG[16]; // general purpose registers, referred to as Vx where x range from 0-F
uint16_t PC_REG; // Program counter
uint8_t SP_REG; // Stack pointer
uint16_t I_REG; // 16 bit register, for storing memory addresses

uint16_t STACK[16];
uint8_t STACK_PTR;

void cpu_init();
void cpu_run();

#endif
