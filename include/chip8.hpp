#ifndef CHIP_8_H
#define CHIP_8_H

#include "program.hpp"

class Chip8 {
private:
	unsigned short I;
	unsigned short pc;
	unsigned short op;
	unsigned char sp;
	unsigned char delay;
	unsigned char sound;

	unsigned int cycles = 0;

	unsigned short memory[4096];
	unsigned char gfx[64 * 32];
	char V[16];
	unsigned short stack[16];
	bool keys[16];

	unsigned char fontSet[80] = {
        0xF0, 0x90, 0x90, 0x90, 0xF0, // 0
        0x20, 0x60, 0x20, 0x20, 0x70, // 1
        0xF0, 0x10, 0xF0, 0x80, 0xF0, // 2
        0xF0, 0x10, 0xF0, 0x10, 0xF0, // 3
        0x90, 0x90, 0xF0, 0x10, 0x10, // 4
        0xF0, 0x80, 0xF0, 0x10, 0xF0, // 5
        0xF0, 0x80, 0xF0, 0x90, 0xF0, // 6
        0xF0, 0x10, 0x20, 0x40, 0x40, // 7
        0xF0, 0x90, 0xF0, 0x90, 0xF0, // 8
        0xF0, 0x90, 0xF0, 0x10, 0xF0, // 9
        0xF0, 0x90, 0xF0, 0x90, 0x90, // A
        0xE0, 0x90, 0xE0, 0x90, 0xE0, // B
        0xF0, 0x80, 0x80, 0x80, 0xF0, // C
        0xE0, 0x90, 0x90, 0x90, 0xE0, // D
        0xF0, 0x80, 0xF0, 0x80, 0xF0, // E
        0xF0, 0x80, 0xF0, 0x80, 0x80  // F
    };

	unsigned short getNextOp();

	void SYS(unsigned short op);
	void CLS();
	void RET();
	void JP_addr(unsigned short op);
	void CALL_addr(unsigned short op);
	void SE_Vx_byte(unsigned short op, unsigned char x);
	void SNE_Vx_byte(unsigned short op, unsigned char x);
	void SE_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void LD_Vx_byte(unsigned short op, unsigned char x);
	void ADD_Vx_byte(unsigned short op, unsigned char x);
	void LD_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void OR_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void AND_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void XOR_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void ADD_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void SUB_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void SHR_Vx(unsigned short op, unsigned char x);
	void SUBN_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void SHL_Vx(unsigned short op, unsigned char x);
	void SNE_Vx_Vy(unsigned short op, unsigned char x, unsigned char y);
	void LD_I_addr(unsigned short op);
	void JP_V0_addr(unsigned short op);
	void RND_Vx_byte(unsigned short op, unsigned char x);
	void DRW_Vx_Vy_nibble(unsigned short op, unsigned char x, unsigned char y);
	void SKP_Vx(unsigned short op, unsigned char x);
	void SKNP_Vx(unsigned short op, unsigned char x);
	void LD_Vx_DT(unsigned short op, unsigned char x);
	void LD_Vx_K(unsigned short op, unsigned char x);
	void LD_DT_Vx(unsigned short op, unsigned char x);
	void LD_ST_Vx(unsigned short op, unsigned char x);
	void ADD_I_Vx(unsigned short op, unsigned char x);
	void LD_F_Vx(unsigned short op, unsigned char x);
	void LD_B_Vx(unsigned short op, unsigned char x);
	void LD_I_Vx(unsigned short op, unsigned char x);
	void LD_Vx_I(unsigned short op, unsigned char x);

	void opLookUp(unsigned short op);

public:
	bool drawFlag;

	Chip8();

	void loadProgram(program_t *program);

	void emulateCycle();

	unsigned char getPixel(int x, int y);
};

#endif
