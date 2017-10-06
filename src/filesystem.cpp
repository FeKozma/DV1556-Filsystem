#include "filesystem.h"

FileSystem::FileSystem(int fileSize) {
	mMemblockDevice = new MemBlockDevice(fileSize);
	curFolder = new inode(this->curFolder, "");
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

bool FileSystem::createFile(std::string name, std::string path)
{
	int x = 1;
	return curFolder->addFile(name, x, path);
}

bool FileSystem::createFolderi(std::string name, std::string path)
{
	return curFolder->addFolder(name, path);
}


/* Please insert your code */