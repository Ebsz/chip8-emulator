#include "screen.h"

void draw_sprite_line(int x, int y, uint8_t l);
void render_pixel(int x, int y);
int screen_pixels[WINDOW_WIDTH][WINDOW_HEIGHT] = {0}; // defines every pixel on the screen: 0 = white, 1 = black

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

// TODO: implement with RenderFillRects() - should be faster
void screen_render() 
{
	SDL_RenderClear(renderer);	

	for (int i = 0; i < WINDOW_WIDTH; i++) {
		for (int j = 0; j < WINDOW_HEIGHT; j++) {
			if (screen_pixels[i][j] == 0) {
				SDL_SetRenderDrawColor(renderer, 255, 255, 255, 0);
			} else {
				SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			}
				render_pixel(i,j);
		}
	}
	SDL_RenderPresent(renderer);
}

void render_pixel (int x, int y)
{
	SDL_Rect r;
	r.x = x* WINDOW_SCALE;
	r.y = y* WINDOW_SCALE;
	r.w = WINDOW_SCALE;
	r.h = WINDOW_SCALE;

	SDL_RenderFillRect(renderer, &r);
}

void screen_clear()
{
	for (int i = 0; i < WINDOW_WIDTH; i++) {
		for (int j = 0; j < WINDOW_HEIGHT; j++) {
			screen_pixels[i][j] = 0;
		}
	}
}

// Draw a sprite at (x, y) 
// a sprite is defined by a 8 bit by 15 array
int screen_draw_sprite(int x, int y, uint8_t sprite[])
{
	// Iterate over each line in the sprite
	for (int i = 0; i < SPRITE_SIZE; i++) {
		if (sprite[i] != 0x0) {
			draw_sprite_line(x, y+i, sprite[i]);
		}
	}
}

// Draw a single line in the sprite
void draw_sprite_line(int x, int y, uint8_t line) 
{
	for (int i = 0; i < 8; i++) { // each bit in the line
		if ((line >> (7-i) ) & 1) { // get bit
			screen_pixels[x+i][y] =1;
		}
	}
}

