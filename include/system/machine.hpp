#ifndef MACHINE_H
#define MACHINE_H

#include "cpu/chip8.hpp"
#include "graphics/screen.hpp"

class Machine {
private:
	Chip8 *cpu;

	Screen *screen;

	bool quit;

	void cycle();

public:
	Machine(Chip8 *, Screen *);

	void start();
};

#endif
