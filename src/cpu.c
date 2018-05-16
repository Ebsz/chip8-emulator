#include <stdio.h>
#include <SDL2/SDL.h>
#include <stdlib.h> // srand(), rand()
#include <time.h>	// time()

#include "cpu.h"
#include "mem.h"
#include "screen.h"
#include "instructions.h"

static void execute();

// Instructions are based in their most significant digit
// Defined in instructions.h
void (*op_base[0x10]) (uint16_t) = { 
			op_zero, 	//0x0
			op_jp, 		//0x1
			op_call, 	//0x2
			op_se, 		//0x3
			op_sne, 	//0x4
			op_se_reg, 	//0x5
			op_ld, 		//0x6
			op_add, 	//0x7
			op_8, 		//0x8
			op_9, 		//0x9
			op_ld_i, 	//0xA
			op_B, 		//0xB
			op_rnd, 	//0xC
			op_drw, 	//0xD
			op_skp, 	//0xE
			op_F, 		//0xF
};

void cpu_init()
{
	srand(time(NULL));

	PC_REG = 0x200;
	SP_REG = 0x0;
	STACK_PTR = 0;

	SOUND_TIMER = 0;
	DELAY_TIMER = 0;

	printf("CPU OK!\n");

	cpu_run();
}

// TODO: define as static, as it is only to be accessed within cpu.c?
void cpu_run()
{
	printf("\n");
	int running = 1;
	SDL_Event e;

	while(running) {
		while(SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) running = 0;
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = 0;
		}

		execute();
	}
}

// TODO: define as static, as it is only to be accessed within cpu.c?
static void execute()
{
	uint8_t op1 = mem_read_byte(PC_REG++);
	uint8_t op2 = mem_read_byte(PC_REG++);
	uint16_t op = op1*0x100 + op2;

	printf("%x\n", op);

	uint8_t base = op/0x1000;

	(*op_base[base]) (op);
}

