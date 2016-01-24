#ifndef MACHINE_H
#define MACHINE_H

#include "chip8.hpp"
#include "screen.hpp"

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
