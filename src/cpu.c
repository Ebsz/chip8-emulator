#include <stdio.h>
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
void op_7(uint16_t op);
void op_8(uint16_t op);
void op_9(uint16_t op);
void op_ld_i(uint16_t op);
void op_B(uint16_t op);
void op_C(uint16_t op);
void op_drw(uint16_t op);
void op_E(uint16_t op);
void op_F(uint16_t op);


// Instructions are based in their most significant digit
void (*op_base[0x10]) (uint16_t) = 
		  { op_zero, 	//0x0xxx
			op_jp, 		//0x1xxx
			op_call, 	//0x2xxx
			op_se, 		//0x3xxx
			op_sne, 	//0x4xxx
			op_se_reg, 	//0x5xxx
			op_ld, 		//0x6xxx
			op_7, 		//0x7xxx
			op_8, 	//0x8xxx
			op_9, //0x9xxx
			op_ld_i, //0xAxxx
			op_B, //0xBxxx
			op_C, //0xCxxx
			op_drw, //0xDxxx
			op_E, //0xExxx
			op_F, //0xFxxx
};

void cpu_init()
{
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
void op_7(uint16_t op)
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
		
		case 3: 	// 8xy1 - Vx = Vx XOR Vy
			GP_REG[x] = GP_REG[x] ^ GP_REG[y];
			break;

		case 4:
			break;
	}
}

// 9xxx
void op_9(uint16_t op)
{
	getchar();
}

// Annn - I-reg = nnn
void op_ld_i(uint16_t op)
{
	uint16_t n = op%0x1000;
	I_REG = n;
}

// Bxxx
void op_B(uint16_t op)
{
	getchar();
}

// Cxxx
void op_C(uint16_t op)
{
	getchar();
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

// Exxx
void op_E(uint16_t op)
{
	getchar();
}

// Fxxx
void op_F(uint16_t op)
{
	getchar();
}
