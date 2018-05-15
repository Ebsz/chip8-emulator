#include "mem.h"
#include "cpu.h"
#include "screen.h"

int main()
{
	mem_load_rom();
	screen_init();
	
	cpu_init();
	return 0;
}
