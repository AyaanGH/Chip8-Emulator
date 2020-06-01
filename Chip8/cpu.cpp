#pragma once
#include <fstream>
#include "cpu.h"
#include <string>




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