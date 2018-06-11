#include "mem.h"
#include "sprites.h"

#include <stdio.h>

void mem_init(char* path)
{
	load_sprites();

	FILE* rom;
	rom = fopen(path, "rb");

	if (rom == NULL) {
		printf("Could not load ROM!");
	} else {
		fread(mem+0x200, sizeof(uint8_t) * 0x1000,1,rom);
		printf("MEM OK!\n");	
	}
	fclose(rom);
}

uint8_t mem_read_byte(uint16_t addr)
{
	if (addr >= 0x1000) {
		printf("Trying to access memory beyond 0x1000!");
		return 0x0;
	}
	return mem[addr];
}

//TODO: add checks for invalid addr
void mem_write_byte(uint16_t addr, uint8_t byte)
{
	if (addr >= 0x1000) {
		printf("Trying to access memory beyond 0x1000!");
	} else {
		mem[addr] = byte;
	}
}

void print_mem()
{
	for (int i = 0; i< 0x100; i++) {
		for (int j = 0; j < 0x10; j++) {
			printf("%x ", mem[j+0x10*i]);
		}
		printf("\n");
	}
}
