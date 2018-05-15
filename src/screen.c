#include "screen.h"

void draw_sprite_line(int x, int y, uint8_t l);
void draw_pixel(int x, int y);

void screen_init()
{
	// Init SDL
	if(SDL_Init(SDL_INIT_VIDEO) != 0) {
		printf("SDL error: %s", SDL_GetError());
	}

	// Init window
	window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED,
	WINDOW_WIDTH*WINDOW_SCALE, WINDOW_HEIGHT*WINDOW_SCALE, 0);
	
	if (window == NULL) {
		printf("Could not init window: %s", SDL_GetError());
	}

	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

	screen_clear();
}

void screen_clear()
{
	SDL_SetRenderDrawColor(renderer, 255,255,255,0);
	SDL_RenderClear(renderer);	
	
	SDL_RenderPresent(renderer);
}

// Draw a sprite at (x, y) 
// a sprite is defined by a 8 bit by 15 array
int screen_draw_sprite(int x, int y, uint8_t sprite[])
{
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	// Iterate over each line in the sprite
	for (int i = 0; i < SPRITE_SIZE; i++) {
		if (sprite[i] != 0x0) {
			draw_sprite_line(x, y+i, sprite[i]);
		}
	}

	SDL_RenderPresent(renderer);
}


// Draw a single line in the sprite
void draw_sprite_line(int x, int y, uint8_t line) 
{
	for (int i = 0; i < 8; i++) { // each bit in the line
		if ((line >> (7-i) ) & 1) { // get bit
			draw_pixel(x+i, y);
		}
	}
}

void draw_pixel (int x, int y)
{
	SDL_Rect r;
	r.x = x* WINDOW_SCALE;
	r.y = y* WINDOW_SCALE;
	r.w = WINDOW_SCALE;
	r.h = WINDOW_SCALE;

	SDL_RenderFillRect(renderer, &r);
}
