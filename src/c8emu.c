#include <stdio.h>
#include <stdbool.h>
#include "mem.h"
#include "cpu.h"
#include "screen.h"

int main(int argc, char* argv[])
{
	if (argc != 2) {
		printf("usage: c8emu input_program\n");
		return 0;
	}

	if (!mem_init(argv[1])) {
		return 0;	
	}
	
	screen_init();
	cpu_init();

	cpu_run();
	return 0;
}
