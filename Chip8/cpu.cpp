#pragma once
#include <fstream>
#include "cpu.h"
#include <string>
#include <chrono>
#include<random>




//Fonts

uint8_t fontset[80] =
{
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

//Constructor
Cpu::Cpu()
	: randomEngine(std::chrono::system_clock::now().time_since_epoch().count())
{
	programCounter = 0x200;


	//Load fonts into memory at position 0x50

	for (int i = 0; i < 80; i++)
	{
		
		memory[0x50 + i] = fontset[i];
	}



	//Init random

	randByte = std::uniform_int_distribution<uint8_t>(0, 255U);

	


}



void Cpu::loadROM(std::string filename)
{

	//Create input binary filestream and set stream pos indicator to the end
	std::ifstream rom_file(filename, std::ios::binary | std::ios::ate);

	if (rom_file.is_open())
	{

		//Get file size in bytes
		std::streampos file_size = rom_file.tellg();

		//Create a buffer the size of the ROM
		char* buffer = new char[file_size];
		

		//Place read head to start
		rom_file.seekg(0, std::ios::beg);

		//Read binary file into the buffer 
		rom_file.read(buffer, file_size);
		rom_file.close();

		//Copy buffer of ROM into memory
		
		for (long i = 0; i < file_size; i++)
		{
			//First 512 bytes are reserved
			memory[0x200 + i] = buffer[i];
		}

		delete[] buffer;

	}


}


//CLS-Clear Display
void Cpu::OP_00E0()
{
	memset(video, 0, sizeof(video));
}


//RET-Return from subroutine

void Cpu::OP_00E0()
{
	--stackPointer;
	programCounter = stack[stackPointer];

}

//JP addr - Jump to address nnn

void Cpu::OP_1nnn()
{

	programCounter = opcode & 0x0FFFu;

}


//CALL addr - Call subroutine at addr 2nnn

void Cpu::OP_2nnn()
{
	stack[stackPointer] = programCounter;

	++stackPointer;

	programCounter = opcode & 0x0FFFu;


}


//SE Vx, byte - Skip next instruction if Vx = kk.


void Cpu::OP_3xkk()
{

	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t hex_byte = opcode & 0x00FFu;
	if (registers[vx] == hex_byte)
	{
		programCounter += 2;
	}
}


//SNE Vx, byte - Skip next instruction if Vx != kk.

void Cpu::OP_4xkk()
{

	uint8_t vx = (opcode & 0x0F00u) >> 8;

	uint8_t kk = (opcode & 0x00FFu);

	if (registers[vx] != kk)
	{
		programCounter += 2;
	}
}


//SE Vx, Vy - Skip next instruction if Vx = Vy.
void Cpu::OP_5xy0()

{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u;

	if (registers[vx] == registers[vy])
	{
		programCounter += 2;
	}
}

//LD Vx, byte - Set Vx = kk

void Cpu::OP_6xkk()
{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t kk = (opcode & 0x00FFu);

	registers[vx] = kk;

}

//7xkk - ADD Vx, byte - Set Vx = Vx + kk.

void Cpu::OP_7xkk()
{

	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t kk = (opcode & 0x00FFu);

	registers[vx] += kk;

}


//8xy0 - LD Vx, Vy - Set Vx = Vy.

void Cpu::OP_8ky0()
{

	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u;

	registers[vx] = registers[vy];
}


//8xy1 - OR Vx, Vy - Set Vx = Vx OR Vy.

void Cpu::OP_8ky1()
{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u;

	registers[vx] |= registers[vy];
}


//8xy2 - AND Vx, Vy- Set Vx = Vx AND Vy.

void Cpu::OP_8ky2()
{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u;

	registers[vx] &= registers[vy];
}


//8xy3 - XOR Vx, Vy - Set Vx = Vx XOR Vy.

void Cpu::OP_8ky3()
{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u;

	registers[vx] ^= registers[vy];
}


//8xy4 - ADD Vx, Vy


void Cpu::OP_8ky4()
{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u;

	uint16_t sum = registers[vx] + registers[vy];


	if (sum > 255u)
	{
		registers[0xF] = 1;
	}
	
	else
	{
		registers[0xF] = 0;
	}

	registers[vx] = sum & 0xFFu;
}


//8xy5 - SUB Vx, Vy - Set Vx = Vx - Vy, set VF = NOT borrow.

void Cpu::OP_8ky5()
{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u;



	if (registers[vx] > registers[vy]) 
	{
		registers[0xF] = 1;
	}

	else
	{
		registers[0xF] = 0;
	}

	registers[vx] -= registers[vy];
}


//8xy6 - SHR Vx - Set Vx = Vx SHR 1.


void Cpu::OP_8ky6()
{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[vx] & 0x1u);

	registers[vx] >>= 1;



}


//8xy7 - SUBN Vx, Vy- Set Vx = Vy - Vx, set VF = NOT borrow.


void Cpu::OP_8xy7()
{
	
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u; 


	if (registers[vy] > registers[vx])
	{
		registers[0xF] = 1;
	}

	else
	{
		registers[0xF] = 0;
	}

	registers[vx] = registers[vy] - registers[vx];
}



//8xyE - SHL Vx - Set Vx = Vx SHL 1.
void Cpu::OP_8xyE()
{

	uint8_t vx = (opcode & 0x0F00u) >> 8u;

	registers[0xF] = (registers[vx] & 0x80u) >> 7;

    registers[vx] <<=  1;

}


//9xy0 - SNE Vx, Vy, Skip next instruction if Vx != Vy.


void Cpu::OP_9xy0()
{
	uint8_t vx = (opcode & 0x0F00u) >> 8u;
	uint8_t vy = (opcode & 0x00F0u) >> 4u;

	if (registers[vx] != registers[vy])
	{
		programCounter += 2;
	}
}



//Annn - LD I, addr, Set I = nnn.


void Cpu::OP_Annn()
{
	indexRegister = (opcode & 0x0FFFu);
}


//Bnnn - JP V0, addr, Jump to location nnn + V0.


void Cpu::OP_Bnnn()
{
	uint16_t address = opcode & 0x0FFFu;

	programCounter += registers[0] + address;
}



//Cxkk - RND Vx, byte, Set Vx = random byte AND kk.

void Cpu::OP_Cxkk()
{

	uint8_t vx = (opcode & 0x0F00u) >> 8u;

	uint8_t kk = (opcode & 0x00FFu);

	registers[vx] = randByte(randomEngine) & kk;

}


