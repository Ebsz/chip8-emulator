#include "mem.h"
#include <stdio.h>

void mem_load_rom()
{
	char* rom_loc = "examples/PONG";

	FILE* rom;
	rom = fopen(rom_loc, "rb");

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

void print_mem()
{
	for (int i = 0; i< 0x100; i++) {
		for (int j = 0; j < 0x10; j++) {
			printf("%x ", mem[j+0x10*i]);
		}
		printf("\n");
	}
}
