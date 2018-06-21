#include <stdio.h>
#include "mem.h"
#include "sprites.h"

uint8_t mem[0x1000] = {0};

bool mem_init(char* path)
{
	load_sprites();

	FILE* rom;
	rom = fopen(path, "rb");

	if (rom == NULL) {
		printf("Could not load file!\n");
		return false;
	}

	// Files are loaded starting at 0x200, as expected by most programs
	fread(mem+0x200, sizeof(uint8_t) * 0x1000,1,rom);
	fclose(rom);

	printf("MEM OK!\n");
	return true;
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
		printf("Trying to write to memory beyond 0x1000!");
	} else {
		mem[addr] = byte;
	}
}

