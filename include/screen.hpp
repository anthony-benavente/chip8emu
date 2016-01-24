#ifndef SCREEN_H
#define SCREEN_H
#include <SDL2/SDL.h>
#include "chip8.hpp"

class Screen {

private:
	unsigned int width;

	unsigned int height;

	unsigned int scale;

public:
	unsigned int *buffer;

	Screen(unsigned int _width,
		   unsigned int _height,
		   unsigned int _scale);

	~Screen();

	void setPixel(int x, int y, unsigned int val);

	unsigned int getPixel(int x, int y);

	void update(Chip8& cpu);

	void render(SDL_Renderer *);
};

#endif
