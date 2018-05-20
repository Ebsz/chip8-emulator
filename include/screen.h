#ifndef SCREEN_H
#define SCREEN_H

#include <SDL2/SDL.h>

#define WINDOW_WIDTH 	64
#define WINDOW_HEIGHT 	32
#define WINDOW_SCALE  	10

#define WINDOW_TITLE "C8EMU"

#define SPRITE_SIZE 15

void screen_init();
void screen_render();

// Draw sprite on screen - return 0 if no collision, 1 if collision
int screen_draw_sprite(int x, int y, uint8_t sprite[]);
void screen_clear();

SDL_Window* window;
SDL_Renderer* renderer;

#endif
