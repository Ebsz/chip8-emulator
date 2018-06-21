#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>
#include <stdbool.h>

#define WINDOW_WIDTH 	64
#define WINDOW_HEIGHT 	32
#define WINDOW_SCALE  	10

#define WINDOW_TITLE "Chip-8 Emulator"

void screen_init();
void screen_render();

// Draw sprite on screen at (x, y) - returns true if this causes any pixels to be erased, false if not
bool screen_draw_sprite(int x, int y, uint8_t sprite[]);

void screen_clear();

SDL_Window* window;
SDL_Renderer* renderer;

#endif
