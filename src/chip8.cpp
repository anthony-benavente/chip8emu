#include "chip8.hpp"

#include <stdlib.h>
#include <time.h>

Chip8::Chip8() {
	pc = 0x200;
	sp = 0;
	I = 0;
	op = 0;
	delay = 0;
	sound = 0;
	drawFlag = false;

	for (int i = 0; i < 4096; i++) {
		if (i < 80) {
			memory[i] = fontSet[i];
		} else {
			memory[i] = 0;
		}
	}

	for (int i = 0; i < 16; i++) {
		V[i] = 0;
		stack[i] = 0;
		keys[i] = false;
	}

	srand(time(NULL));
}

void Chip8::loadProgram(unsigned char *program, int n) {
	for (int i = 0x200; i < n; i++) {
		memory[i] = program[i];
	}
}


void Chip8::emulateCycle() {
	op = Chip8::getNextOp();
}

unsigned short Chip8::getNextOp() {
	return (memory[pc] << 8) | memory[pc + 1];
}

void Chip8::SYS(unsigned short op) {
	pc = op & 0xfff;
}

void Chip8::CLS() {
	for (int i = 0; i < 4096; i++) {
		gfx[i] = 0;
	}
}

void Chip8::RET() {
	pc = stack[sp--];
}

void Chip8::JP_addr(unsigned short op) {
	pc = op & 0xfff;
}

void Chip8::CALL_addr(unsigned short op) {
	stack[++sp] = pc;
	pc = op & 0xfff;
}

void Chip8::SE_Vx_byte(unsigned short op, unsigned char x) {
	if (V[x] == (op & 0xff)) {
		pc += 2;
	}
}

void Chip8::SNE_Vx_byte(unsigned short op, unsigned char x) {
	if (V[x] != (op & 0xff)) {
		pc += 2;
	}
}

void Chip8::SE_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	if (V[x] == V[y]) {
		pc += 2;
	}
}

void Chip8::LD_Vx_byte(unsigned short op, unsigned char x) {
	V[x] = op & 0xff;
}

void Chip8::ADD_Vx_byte(unsigned short op, unsigned char x) {
	V[x] += op & 0xff;
}

void Chip8::LD_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[x] = V[y];
}

void Chip8::OR_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[x] |= V[y];
}

void Chip8::AND_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[x] &= V[y];
}

void Chip8::XOR_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[x] ^= V[y];
}

void Chip8::ADD_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[0xf] = V[x] + V[y] > 0xff ? 1 : 0;
	V[x] = (V[x] + V[y]) & 0xff;
}

void Chip8::SUB_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[0xf] = V[x] > V[y] ? 1 : 0;
	V[x] -= V[y];
}

void Chip8::SHR_Vx(unsigned short op, unsigned char x) {
	V[0xf] = V[x] & 0x1;
	V[x] >>= 1;
}

void Chip8::SUBN_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	V[0xf] = V[y] > V[x];
	V[x] = V[y] - V[x];
}

void Chip8::SHL_Vx(unsigned short op, unsigned char x) {
	V[0xf] = V[x] & 0x80;
	V[x] <<= 1;
}

void Chip8::SNE_Vx_Vy(unsigned short op, unsigned char x, unsigned char y) {
	if (V[x] != V[y]) {
		pc += 2;
	}
}

void Chip8::LD_I_addr(unsigned short op) {
	I = op & 0xfff;
}

void Chip8::JP_V0_addr(unsigned short op) {
	pc = (op & 0xfff) + V[0];
}

void Chip8::RND_Vx_byte(unsigned short op, unsigned char x) {
	V[x] = (rand() % 256) & (op & 0xff);
}

void Chip8::DRW_Vx_Vy_nibble(unsigned short op, unsigned char x, unsigned char y) {
	unsigned short xp = V[x];
	unsigned short yp = V[y];
	unsigned short pixel;

	V[0xf] = 0;
	for (int yline = 0; yline < (op & 0xf); yline++) {
		pixel = memory[I + yline];
		for (int xline = 0; xline < 8; xline++) {
			int idx = (yp + yline) * 64 + (xp + xline);
			if ((pixel & (0x80 >> xline)) != 0) {
				if (gfx[idx]) {
					V[0xf] = 1;
				}
				gfx[idx] ^= 1;
			}
		}
	}

	drawFlag = true;
}

void Chip8::SKP_Vx(unsigned short op, unsigned char x) {
	if (keys[(int) V[x]]) {
		pc += 2;
	}
}

void Chip8::SKNP_Vx(unsigned short op, unsigned char x) {
	if (!keys[(int) V[x]]) {
		pc += 2;
	}
}

void Chip8::LD_Vx_DT(unsigned short op, unsigned char x) {
	V[x] = delay;
}

void Chip8::LD_Vx_K(unsigned short op, unsigned char x) {
	// Wait for keypress
}

void Chip8::LD_DT_Vx(unsigned short op, unsigned char x) {
	delay = V[x];
}

void Chip8::LD_ST_Vx(unsigned short op, unsigned char x) {
	sound = V[x];
}

void Chip8::ADD_I_Vx(unsigned short op, unsigned char x) {
	I += V[x];
}

void Chip8::LD_F_Vx(unsigned short op, unsigned char x) {
	I = memory[(int) V[x]];
}

void Chip8::LD_B_Vx(unsigned short op, unsigned char x) {
	memory[I] = (V[x] / 100) % 10;
	memory[I + 1] = (V[x] / 10) % 10;
	memory[I + 2] = (V[x] / 1) % 10;
}

void Chip8::LD_I_Vx(unsigned short op, unsigned char x) {
	for (int i = 0; i < 16; i++) {
		memory[I + i] = V[i];
	}
}

void Chip8::LD_Vx_I(unsigned short op, unsigned char x) {
	for (int i = 0; i < 16; i++) {
		V[i] = memory[I + i];
	}
}
