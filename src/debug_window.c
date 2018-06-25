#include <ncurses.h>
#include <unistd.h>
#include <stdlib.h> //exit()

#include "debug_window.h"
#include "cpu.h"
#include "mem.h"

static void draw_registers();
static void draw_hexdump();

// screen coordinates
int max_x;
int max_y;
int min_x = 2;
int min_y = 1;

void dbwin_init()
{
	initscr(); // init window
	noecho(); // dont echo keypresses
	curs_set(FALSE); // don't show cursor

	getmaxyx(stdscr, max_y, max_x);
	if (has_colors() == TRUE) {
		start_color(); // use color
		init_pair(1, COLOR_BLACK, COLOR_RED);
	}

}

void dbwin_update(uint16_t op)
{
	mvprintw(min_y, min_x, "opcode: %04x", op);
	draw_registers();
	draw_hexdump();	

	refresh();

	if(getch() != '\n') {
		dbwin_exit();
		exit(0);
	}
}

static void draw_registers()
{
	for (int i=0; i<16;i++) {
		mvprintw(3+i,min_x,"V%x: %02x", i, GP_REG[i]);
	}

	mvprintw(3,12,"PC: %x", PC_REG);
	mvprintw(4,12,"SP: %x", SP_REG);
	mvprintw(5,12,"I: %x", I_REG);
}

static void draw_hexdump()
{
	int ptr = 0x200;
	int start_pos = max_x - (8*5)-2;

	for (int i = 0; i<40; i++) {
		for (int j=0; j<8;j++) {
			int op1 = mem_read_byte(ptr++);
			int op2 = mem_read_byte(ptr++);
			int op = op1*0x100 + op2;
			
			if (ptr == PC_REG) {
				attron(COLOR_PAIR(1));
			}
			mvprintw(min_y + i,start_pos+j*5, "%04x", op);
			attroff(COLOR_PAIR(1));
			mvprintw(min_y+i,start_pos+j*5+4," ");
		}
	}
}

void dbwin_exit()
{
	endwin();
}
