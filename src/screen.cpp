#include "screen.hpp"

#include <cstdlib>

Screen::Screen(unsigned int _width, unsigned int _height) {
	buffer = (unsigned int *) malloc(sizeof(int) * _width * _height);
	width = _width;
	height = _height;
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
