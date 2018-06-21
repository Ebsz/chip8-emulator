#include "screen.h"
#include "sprites.h"

bool draw_sprite_line(int x, int y, uint8_t l);
void render_pixel(int x, int y);
int screen_pixels[WINDOW_WIDTH][WINDOW_HEIGHT] = {0}; // defines every pixel on the screen: 0 = white, 1 = black

void check_pixel_bounds(int* x, int* y);

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
bool screen_draw_sprite(int x, int y, uint8_t sprite[])
{
	bool erased = false;
	// Iterate over each line in the sprite
	for (int i = 0; i < SPRITE_MAX_SIZE; i++) {
		if (sprite[i] != 0x0) {
			if (draw_sprite_line(x, y+i, sprite[i])) {
				erased = true;
			}
		}
	}

	return erased;
}

// Draw a single line in the sprite
bool draw_sprite_line(int x, int y, uint8_t line) 
{
	bool erased = false;

	for (int i = 0; i < 8; i++) { // each bit in the line
		if ((line >> (7-i) ) & 1) { // get bit
			check_pixel_bounds(&x, &y);
			if (screen_pixels[x+i][y] == 0) {
				screen_pixels[x+i][y] = 1;
			} else {
				screen_pixels[x+i][y] = 0;
				erased = true;
			}
		}
	}

	return erased;
}

void check_pixel_bounds(int* x, int* y)
{
	if (*x > WINDOW_WIDTH) {
		*x = *x % WINDOW_WIDTH;
	}

	if (*y > WINDOW_HEIGHT) {
		*y = *y% WINDOW_HEIGHT;
	}
}
