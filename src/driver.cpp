#include <iostream>
#include <SDL2/SDL.h>

#include "chip8.hpp"
#include "screen.hpp"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCALE 10

Chip8 myChip8;
Screen screen(SCREEN_WIDTH, SCREEN_HEIGHT, SCALE);

int display_width = SCREEN_WIDTH * SCALE;
int display_height = SCREEN_HEIGHT * SCALE;

void update();
void render(SDL_Renderer *);
program_t *getProgram(const char *path);
void free_program(program_t *);

int main(int argc, char **argv) {
	program_t *program = getProgram(argv[1]);
	myChip8.loadProgram(program);
	free_program(program);

	SDL_Window *window = NULL;
	SDL_Renderer *renderer = NULL;

	if (SDL_Init(SDL_INIT_VIDEO) < 0) {
		fputs("Failed to initialize SDL!\n", stderr);
	} else {
		window = SDL_CreateWindow("Chip8 Emulator - Anthony Benavente",
			SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
			display_width, display_height,
			SDL_WINDOW_SHOWN);
		if (!window) {
			fputs("Window could not be created!\n", stderr);
		} else {
			renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    		SDL_RenderClear( renderer );

			bool quit = false;
			SDL_Event e;
			while (!quit) {
				while (SDL_PollEvent(&e) != 0) {
					quit = e.type == SDL_QUIT;
				}

				update();
				render(renderer);
			}
		}
	}

   SDL_DestroyWindow( window );
   SDL_Quit();

	return 0;
}

void update() {
	myChip8.emulateCycle();
	if (myChip8.drawFlag) screen.update(myChip8);
}

void render(SDL_Renderer *renderer) {
	if (myChip8.drawFlag) {
		screen.render(renderer);
		myChip8.drawFlag = false;
	}
	SDL_RenderPresent(renderer);
}

program_t *getProgram(const char *path) {
	FILE *fp = fopen(path, "rb");
	if (!fp) {
		fputs("Err reading file\n", stderr);
		exit(1);
	}

	fseek(fp, 0, SEEK_END);
	size_t size = ftell(fp);
	rewind(fp);

	printf("Program size: %d bytes\n", size);

	program_t *result = (program_t *)malloc(sizeof(program_t));
	result->data = (unsigned char *) malloc(sizeof(char) * size);
	result->size = size;
	if (result->size != fread(result->data, 1, size, fp)) {
		fputs("Err copying file to buffer\n", stderr);
		exit(2);
	}
	fclose(fp);

	return result;
}

void free_program(program_t *program) {
	free(program->data);
	free(program);
}
