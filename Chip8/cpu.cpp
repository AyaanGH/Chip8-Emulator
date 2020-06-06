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