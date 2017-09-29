#include "filesystem.h"
//för testning
#include <iostream>

FileSystem::FileSystem() {
	
}

FileSystem::~FileSystem() {

}


// detta är en testfunktion VARNING testfunktion
int FileSystem::createFileOn(int blocknr)
{
	int nrChar = 62;
	

	std::string repString = "Det var en gang en pirat som borjade beratta en saga som lod: ";
	std::string storeString;
	for (int i = 0; (i + repString.length()) < 512; i += repString.length())
	{
		storeString += repString;
	}

	for (int i = storeString.length(); i < 512; i++)
	{
		storeString += "0";
	}
	std::cout << std::to_string(storeString.length()) + "\n";
	int retVal = this->mMemblockDevice.writeBlock(blocknr, storeString);
	return retVal;
}


// detta är en testfunktion VARNING testfunktion
std::string FileSystem::viewFileOn(int blocknr)
{
	Block retBlock = mMemblockDevice.readBlock(blocknr);
	
	return retBlock.toString();
}


/* Please insert your code */