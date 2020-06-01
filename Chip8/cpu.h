#pragma once

#include <cstdint>
class Cpu
{


	//Public data members
	public:

		uint8_t registers[16]; //Registers
		uint8_t memory[0xFFF];
		uint16_t indexRegister;
		uint16_t programCounter; //Starts at address 0x200
		uint16_t stack[16];
		uint8_t stackPointer;
		uint8_t delayTimer;
		uint8_t soundTimer;
		uint8_t keypad[16];
		uint32_t video[64 * 32];
		uint16_t opcode;

	
	private:
		void loadROM(std::string filename);



};

