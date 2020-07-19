#pragma once
#include<chrono>
#include <random>
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
		
		std::default_random_engine randomEngine;

		std::uniform_int_distribution<uint8_t> randByte;
		
		Cpu();


		//OPCODES

		 void OP_00E0();

		 void OP_1nnn();

		 void OP_2nnn();

		 void OP_3xkk();

		 void OP_4xkk();

		 void OP_5xy0();

		 void OP_6xkk();

		 void OP_7xkk();

		 void OP_8ky0();

		 void OP_8ky1();

		 void OP_8ky2();

		 void OP_8ky3();

		 void OP_8ky4();

		 void OP_8ky5();

		 void OP_8ky6();

		 void OP_8xy7();

		 void OP_8xyE();

		 void OP_9xy0();

		 void OP_Annn();

		 void OP_Bnnn();

		 void OP_Cxkk();

		 void OP_Dxyn();

		 void OP_Ex9E();

		 void OP_ExA1();

		 void OP_Fx07();

		 void OP_Fx0A();

		 void OP_Fx15();

		 void OP_Fx18();

		 void OP_Fx1E();

		 void OP_Fx29();

		 void OP_Fx33();

		 void OP_Fx55();

		 void OP_Fx65();




		

	private:
		void loadROM(std::string filename);



};

