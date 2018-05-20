#include "mem.h"
#include "cpu.h"
#include "screen.h"

int main()
{
	char rom_path [] = "examples/PONG2"; 

	mem_load_rom(rom_path);
	screen_init();
	
	cpu_init();
	return 0;
}
