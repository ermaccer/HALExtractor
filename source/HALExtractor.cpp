// HALExtractor.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <fstream>
#include <memory>
#include <windows.h>


struct HALHeader {
	int          header; // APUK
	unsigned int numberOfFiles;
	char         pad[24];
};

struct HALEntry {
	unsigned int  size;
	unsigned long offset;
	int          unknown[2];
	char         filename[48];
};

int main(int argc, char* argv[])
{

	if (argc != 2) {
		std::cout << "Usage halextract <input>" << std::endl;
		return 1;
	}

	std::ifstream pFile(argv[1], std::ifstream::binary);

		if (!pFile)
			printf("ERROR: Could not open %s!", argv[1]);

	if (pFile)
	{
		CreateDirectory((LPCSTR)"extract", NULL);
		HALHeader hal;
		pFile.read((char*)&hal, sizeof(HALHeader));


		if (hal.header != 'KUPA')
		{
			printf("ERROR: %s is not an APUK archive! \n", argv[1]);
			return 1;
		}

	
		for (int i = 0; i < hal.numberOfFiles ; ++i)
		{
			// reading
			HALEntry halFile;
			unsigned long pos = pFile.tellg();
			pFile.read((char*)&halFile, sizeof(HALEntry));
	
			pFile.seekg(halFile.offset, pFile.beg);

			printf("Offset %X Extracting %s \n",halFile.offset, halFile.filename);

			// writing
			std::string output = halFile.filename;
			output.insert(0, "extract\\");
			std::ofstream oFile(output, std::ofstream::binary);
			auto dataSize = (std::streampos)halFile.size;
			auto dataBuff = std::make_unique<char[]>((long)dataSize);
			pFile.read(dataBuff.get(), dataSize);
			oFile.write(dataBuff.get(), dataSize);

			//reset to first member
			pFile.seekg(pos + sizeof(HALEntry), pFile.beg);


		}

	}
	return 0;
}
