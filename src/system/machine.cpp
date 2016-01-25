#include "system/machine.hpp"

#include <SDL2/SDL.h>

Machine::Machine(Chip8 *_cpu, Screen *_screen) {
	cpu = _cpu;
	screen = _screen;
	quit = false;
}

void Machine::start() {
	screen->initSDL();

	SDL_Event e;
	while(!quit) {
		while (SDL_PollEvent(&e) != 0) {
			quit = e.type == SDL_QUIT;
		}
		cycle();
	}
}

void Machine::cycle() {
	cpu->emulateCycle();
	screen->update(cpu);
	screen->render();
}
