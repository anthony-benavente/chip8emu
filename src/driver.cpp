#include <iostream>
#include <SDL2/SDL.h>

#include "cpu/chip8.hpp"
#include "graphics/screen.hpp"
#include "system/machine.hpp"
#include "program/program.hpp"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCALE 10

Chip8 myChip8;
Screen screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCALE);

int main(int argc, char **argv) {
	program_t *program = getProgram(argv[1]);
	myChip8.loadProgram(program);
	free_program(program);

	Machine machine(myChip8, screen);
	machine.start();

	return 0;
}
