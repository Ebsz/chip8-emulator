#include "mem.h"
#include "cpu.h"
#include "screen.h"

int main()
{
	char rom_path [] = "examples/PONG2"; 

	mem_init(rom_path);
	screen_init();

	cpu_init();
	cpu_run();
	return 0;
}
