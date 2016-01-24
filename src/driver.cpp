#include <iostream>
#include <GL/glut.h>

#include "chip8.hpp"

#define SCREEN_WIDTH 64
#define SCREEN_HEIGHT 32
#define SCALE 10

Chip8 myChip8;

int display_width = SCREEN_WIDTH * SCALE;
int display_height = SCREEN_HEIGHT * SCALE;


#define DRAWWITHTEXTURE
typedef uint8_t u8;

u8 screenData[SCREEN_HEIGHT][SCREEN_WIDTH][3];

void display();
void reshape_window(GLsizei w, GLsizei h);
void keyUp(unsigned char key, int x, int y);
void keyDown(unsigned char key, int x, int y);
void setupTexture();
program_t *getProgram(const char *path);
void free_program(program_t *);

int main(int argc, char **argv) {

	program_t *program = getProgram(argv[1]);
	myChip8.loadProgram(program);
	free_program(program);

	// Setup OpenGL
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowSize(display_width, display_height);
	glutInitWindowPosition(320, 320);
	glutCreateWindow("Chip8 Emulator - Anthony Benavente");

	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape_window);
	// glutKeyboardFunc(keyDown);
	// glutKeyboardUpFunc(keyUp);

	setupTexture();

	glutMainLoop();

	return 0;
}

void setupTexture() {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			for (int i = 0; i < 3; i++) {
				screenData[y][x][i] = 0;
			}
		}
	}

	glTexImage2D(GL_TEXTURE_2D, 0, 3, SCREEN_WIDTH, SCREEN_HEIGHT, 0,
				 GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *) screenData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glEnable(GL_TEXTURE_2D);
}

void updateTexture() {
	for (int y = 0; y < SCREEN_HEIGHT; y++) {
		for (int x = 0; x < SCREEN_WIDTH; x++) {
			if (myChip8.getPixel(x, y)) {
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 255;
			} else {
				screenData[y][x][0] = screenData[y][x][1] = screenData[y][x][2] = 0;
			}
		}
	}

	glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, SCREEN_WIDTH, SCREEN_HEIGHT,
					GL_RGB, GL_UNSIGNED_BYTE, (GLvoid *) screenData);
	glBegin( GL_QUADS );
		glTexCoord2d(0.0, 0.0);		glVertex2d(0.0,			  0.0);
		glTexCoord2d(1.0, 0.0); 	glVertex2d(display_width, 0.0);
		glTexCoord2d(1.0, 1.0); 	glVertex2d(display_width, display_height);
		glTexCoord2d(0.0, 1.0); 	glVertex2d(0.0,			  display_height);
	glEnd();
}

void display() {
	myChip8.emulateCycle();

	if (myChip8.drawFlag) {
		glClear(GL_COLOR_BUFFER_BIT);
		updateTexture();
		glutSwapBuffers();
		myChip8.drawFlag = false;
	}
}

void reshape_window(GLsizei w, GLsizei h) {
	glClearColor(0.0, 0.0, 0.5, 0.0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluOrtho2D(0, w, h, 0);
	glMatrixMode(GL_MODELVIEW);
	glViewport(0, 0, w, h);

	display_width = w;
	display_height = h;
}

void keyboardDown(unsigned char key, int x, int y)
{
	if(key == 27)    // esc
		exit(0);
}

void keyboardUp(unsigned char key, int x, int y) {
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

	for (int i = 0x200; i < size + 0x200; i++) {
		printf("0x%x ", result->data[i - 0x200]);
	}
	printf("\n");

	return result;
}

void free_program(program_t *program) {
	free(program->data);
	free(program);
}
