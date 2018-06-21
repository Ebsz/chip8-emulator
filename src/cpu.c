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
			op_zero,		//0x0
			op_jp,			//0x1
			op_call,		//0x2
			op_se,			//0x3
			op_sne,			//0x4
			op_se_reg,		//0x5
			op_ld,			//0x6
			op_add,			//0x7
			op_vx_vy,		//0x8
			op_sne_vx_vy,	//0x9
			op_ld_i, 		//0xA
			op_jp_nnn_v0, 	//0xB
			op_rnd,			//0xC
			op_drw,			//0xD
			op_skp,			//0xE
			op_F,			//0xF
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
}

void cpu_run()
{
	int running = 1;
	SDL_Event e;

	unsigned int prev_time = SDL_GetTicks();
	unsigned int current_time = 0, delta = 0, cpu_time = 0;

	while(running) {
		current_time = SDL_GetTicks();
		delta += (current_time - prev_time);
		cpu_time += (current_time - prev_time);

		prev_time = current_time;

		if (delta > 1000/60 && (SOUND_TIMER+DELAY_TIMER > 0)) {
			if (SOUND_TIMER > 0) {
				SOUND_TIMER--;
			}
			if (DELAY_TIMER > 0) {
				DELAY_TIMER--;
			}
			delta = 0;
		}
		
		// execution is restricted to at most once every 1 ms
		if (cpu_time > 1) {
			execute();
			cpu_time = 0;
		}
		
		// Handle events
		while(SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) running = 0;
			if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) running = 0;
		}

		screen_render();
		
		#ifdef DEBUG
		getchar();
		#endif
	}
}

// Execute a single instruction and advance the program counter 
static void execute()
{
	uint8_t op1 = mem_read_byte(PC_REG++);
	uint8_t op2 = mem_read_byte(PC_REG++);
	uint16_t op = op1*0x100 + op2;

	uint8_t base = op/0x1000;

	#ifdef DEBUG
	printf("%x\n", op);
	for (int i = 0; i<16;i++) {
		printf("V%x: %x  ", i, GP_REG[i]);
	}
	printf("\n");
	#endif

	// execute instruction
	(*op_base[base]) (op);
}

