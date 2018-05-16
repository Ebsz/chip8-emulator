#include <stdio.h>
#include <stdlib.h> // srand(), rand()
#include <time.h>	// time()
#include <SDL2/SDL.h>
#include "cpu.h"
#include "mem.h"
#include "screen.h"

void execute();

// Instructions
void op_zero(uint16_t op);
void op_jp(uint16_t op);
void op_call(uint16_t op);
void op_se(uint16_t op);
void op_sne(uint16_t op);
void op_se_reg(uint16_t op);
void op_ld(uint16_t op);
void op_add(uint16_t op);
void op_8(uint16_t op);
void op_9(uint16_t op);
void op_ld_i(uint16_t op);
void op_B(uint16_t op);
void op_rnd(uint16_t op);
void op_drw(uint16_t op);
void op_skp(uint16_t op);
void op_F(uint16_t op);


int keyboard_map[16] = {
	SDL_SCANCODE_X,		
	SDL_SCANCODE_1,		
	SDL_SCANCODE_2,		
	SDL_SCANCODE_3,		
	SDL_SCANCODE_Q,
	SDL_SCANCODE_W,
	SDL_SCANCODE_E,
	SDL_SCANCODE_A,
	SDL_SCANCODE_S,
	SDL_SCANCODE_D,
	SDL_SCANCODE_Z,
	SDL_SCANCODE_C,
	SDL_SCANCODE_4,
	SDL_SCANCODE_R,
	SDL_SCANCODE_F,
	SDL_SCANCODE_Q,
};


// Instructions are based in their most significant digit
void (*op_base[0x10]) (uint16_t) = 
		  { op_zero, 	//0x0xxx
			op_jp, 		//0x1xxx
			op_call, 	//0x2xxx
			op_se, 		//0x3xxx
			op_sne, 	//0x4xxx
			op_se_reg, 	//0x5xxx
			op_ld, 		//0x6xxx
			op_add, 	//0x7xxx
			op_8, 	//0x8xxx
			op_9, //0x9xxx
			op_ld_i, //0xAxxx
			op_B, //0xBxxx
			op_rnd, //0xCxxx
			op_drw, //0xDxxx
			op_skp, //0xExxx
			op_F, //0xFxxx
};

void cpu_init()
{
	srand(time(NULL));

	PC_REG = 0x200;
	SP_REG = 0x0;
	STACK_PTR = 0;

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
void execute()
{
	uint8_t op1 = mem_read_byte(PC_REG++);
	uint8_t op2 = mem_read_byte(PC_REG++);
	uint16_t op = op1*0x100 + op2;

	printf("%x\n", op);

	uint8_t base = op/0x1000;

	(*op_base[base]) (op);
}

// 00E0 - CLS, 00EE - RET
void op_zero(uint16_t op)
{
	if(op == 0x00E0) {
		screen_clear();
	} else if (op == 0x00EE) {
		PC_REG = STACK[STACK_PTR];

		if (STACK_PTR != 0) {
			STACK_PTR--;
		}
	} 
}

// 1nnn - JP to nnn
void op_jp(uint16_t op)
{
	PC_REG = op % 0x1000;
}

// 2nnn - Call subroutine by incrementing stack, pushing pc, then setting pc to nnn
void op_call(uint16_t op)
{
	if (STACK_PTR > 0) {
		STACK_PTR++;
	} 

	STACK[STACK_PTR] = PC_REG;

	PC_REG = op % 0x1000;
}

// 3xkk - skip next op if Vx == kk
void op_se(uint16_t op)
{
	uint8_t kk = op % 0x100;
	uint8_t x = (op/0x100) % 0x10;

	if (kk == GP_REG[x]) {
		PC_REG += 2;
	}
}

// 4xkk - skip next op if Vx != kk
void op_sne(uint16_t op)
{
	uint8_t kk = op % 0x100;
	uint8_t x = (op/0x100) % 0x10;
	
	if (kk != GP_REG[x]) {
		PC_REG += 2;
	}
}

// 5xy0 - skip next instruction if Vx == Vy
void op_se_reg(uint16_t op)
{
	uint8_t x = (op/0x100) % 0x10;
	uint8_t y = (op/0x10) % 0x10;

	if (GP_REG[x] == GP_REG[y]) {
		PC_REG += 2;
	}
}

// 6xkk - load kk into Vx
void op_ld(uint16_t op)
{
	uint8_t kk = op%0x100;
	uint8_t x = (op/0x100)%0x10;

	GP_REG[x] = kk;
}

// 7xkk -  Vx += kk
void op_add(uint16_t op)
{
	uint8_t kk = op%0x100;
	uint8_t x = (op/0x100)%0x10;

	GP_REG[x] += kk;
}

// 8xyn - Operations with Vx and Vy
void op_8(uint16_t op)
{
	uint8_t n = op % 0x10;
	uint8_t x = (op/0x100) % 0x10;
	uint8_t y = (op/0x10) % 0x10;

	switch (n) {
		case 0: 	// 8xy0 - Vx = Vy
			GP_REG[x] = GP_REG[y];
			break;

		case 1: 	// 8xy1 - Vx = Vx OR Vy
			GP_REG[x] = GP_REG[x] | GP_REG[y];
			break;
			
		case 2:		// 8xy2 - Vx = Vx AND Vy
			GP_REG[x] = GP_REG[x] & GP_REG[y];
			break;
		
		case 3: 	// 8xy3 - Vx = Vx XOR Vy
			GP_REG[x] = GP_REG[x] ^ GP_REG[y];
			break;

		case 4:		// 8xy4 - Vx = Vx + Vy
			if ((GP_REG[x] + GP_REG[y]) > 255) {
				GP_REG[0xF] = 1;
			} else {
				GP_REG[0xF] = 0;
			}
			GP_REG[x] = GP_REG[x] + GP_REG[y];
			break;

		case 5:		// 8xy5 - Vx = Vx - Vy
			if(GP_REG[x] > GP_REG[y]) {
				GP_REG[0xF] = 1;
			} else {
				GP_REG[0xF] = 0;
			}
			GP_REG[x] = GP_REG[x] - GP_REG[y];
			break;

		case 6:		// 8xy6 - shift Vx right
			if (GP_REG[x] & 0x1) { // if LSB is 1
				GP_REG[0xF] = 1;
			} else {
				GP_REG[0xF] = 0;
			}
			
			GP_REG[x] /= 2;

			break;

		case 7:		// 8xy7 - Vx = Vy - Vx
			if (GP_REG[y] > GP_REG[x]) {
				GP_REG[0xF] = 1;
			} else {
				GP_REG[0xF] = 0;
			}
			GP_REG[x] = GP_REG[y] - GP_REG[x];
			break;

		case 0xE:
			if (GP_REG[x] & 0x80) { // if MSB is 1
				GP_REG[0xF] = 1;
			} else {
				GP_REG[0xF] = 0;
			}
			GP_REG[x] *= 2;
			break;
	}
}

// 9xy0 - Skip next instruction if Vx != Vy
void op_9(uint16_t op)
{
	uint8_t x = (op/0x100) % 0x10;
	uint8_t y = (op/0x10) % 0x10;
	
	if (GP_REG[x] != GP_REG[y]) {
		PC_REG += 2;
	}
}

// Annn - I-reg = nnn
void op_ld_i(uint16_t op)
{
	uint16_t n = op%0x1000;
	I_REG = n;
}

// Bnnn - Jump to nnn + V0
void op_B(uint16_t op)
{
	uint16_t n = op%0x1000;
	PC_REG = n + GP_REG[0];

}

// Cxkk - Vx = Random byte AND kk
void op_rnd(uint16_t op)
{
	uint8_t x = (op/0x100)%0x10;
	uint8_t kk = op%0x100;
	uint8_t r = rand()%256;
	
	GP_REG[x] = r & kk;
}

// Dxyn - draw n byte sprite starting at I_REG, at (Vx, Vy)
void op_drw(uint16_t op)
{
	uint8_t n = op %0x10;
	uint8_t x = (op/0x100)%0x10;
	uint8_t y = (op/0x10)%0x10;
	
	uint8_t sprite[SPRITE_SIZE] = {0};

	for(int i = 0; i < n; i++) {
		sprite[i] = mem_read_byte(I_REG+i);
	}

	screen_draw_sprite(GP_REG[x], GP_REG[y], sprite);
		
	getchar();
}

// Exkk - Skip instruction if key is pressed/not pressed
void op_skp(uint16_t op)
{
	uint8_t x = (op/0x100)%0x10;
	uint8_t kk = op%0x100;

	SDL_PumpEvents(); // Update state array
	const Uint8* state = SDL_GetKeyboardState(NULL);

	if (kk == 0x9E) { // skip if key at Vx is pressed
		if (state[keyboard_map[GP_REG[x]]]) {
			PC_REG += 2;
		}
	}

	if (kk == 0xA1) { // skip if key not pressed
		if(!state[keyboard_map[GP_REG[x]]]) {
			PC_REG += 2;
		}
	}

}

// Fxkk
void op_F(uint16_t op)
{
	uint8_t x = (op/0x100)%0x10;
	uint8_t kk = op%0x100;

}
