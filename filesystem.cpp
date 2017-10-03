#include "filesystem.h"

FileSystem::FileSystem(int fileSize) {
	mMemblockDevice = new MemBlockDevice(fileSize);
}

FileSystem::~FileSystem() {
	delete mMemblockDevice;
	delete[] availableBlocks;
}

// detta är en testfunktion VARNING testfunktion
int FileSystem::createFileOn(std::string storeString, int blocknr)
{
	int lengthOfBlock = 512;

	for (int i = storeString.length(); i < lengthOfBlock; i++)
	{
		storeString += "0";
	}
	std::cout << std::to_string(storeString.length()) + "\n";
	int retVal = this->mMemblockDevice->writeBlock(blocknr, storeString);
	return retVal;
}


// detta är en testfunktion VARNING testfunktion
std::string FileSystem::viewFileOn(int blocknr)
{
	Block retBlock = mMemblockDevice->readBlock(blocknr);
	
	return retBlock.toString();
}


/* Please insert your code */