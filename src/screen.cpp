#include "screen.hpp"

Screen::Screen(unsigned int width, unsigned int height) {
	buffer = (int *) malloc(sizeof(int) * width * height);
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
