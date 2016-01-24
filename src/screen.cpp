#include "screen.hpp"
#include "chip8.hpp"

#include <cstdlib>
#include <iostream>
#include <new>

Screen::Screen(unsigned int _width,
			   unsigned int _height,
			   unsigned int _scale) {
	buffer = (unsigned int *) malloc(sizeof(unsigned int) * _width *
	 	_height * _scale);
	width = _width;
	height = _height;
	scale = _scale;

	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			setPixel(x, y, 0);
		}
	}
}

Screen::~Screen() {
	free(buffer);
}

void Screen::setPixel(int x, int y, unsigned int val) {
	buffer[y * width + x] = val;
}

unsigned int Screen::getPixel(int x, int y) {
	return buffer[y * width + x];
}

void Screen::update(Chip8& cpu) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			setPixel(x, y, cpu.getPixel(x, y) * 0xffffff);
		}
	}
}

void Screen::render(SDL_Renderer *renderer) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			int r = (getPixel(x, y) >> 16) & 0xff;
			int g = (getPixel(x, y) >> 8) & 0xff;
			int b = (getPixel(x, y) & 0xff);
			SDL_Rect rect;
			rect.x = x * scale;
			rect.y = y * scale;
			rect.w = scale;
			rect.h = scale;

			SDL_SetRenderDrawColor(renderer, r, g, b, 255);
			SDL_RenderFillRect(renderer, &rect);
		}
	}
}
