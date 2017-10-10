#include "filesystem.h"

FileSystem::FileSystem(int fileSize) {
	mMemblockDevice = new MemBlockDevice(fileSize);
	curFolder = new inode("");
}

FileSystem::~FileSystem() {
	delete mMemblockDevice;
	delete curFolder;
	delete[] availableBlocks;
}

// Detta är en testfunktion VARNING testfunktion
int FileSystem::createFileOn(std::string storeString, int blocknr) {
	int lengthOfBlock = 512;

	for (int i = storeString.length(); i < lengthOfBlock; i++)
	{
		storeString += "0";
	}
	std::cout << std::to_string(storeString.length()) + "\n";
	int retVal = this->mMemblockDevice->writeBlock(blocknr, storeString);
	return retVal;
}

// Detta är en testfunktion VARNING testfunktion
std::string FileSystem::viewFileOn(int blocknr) {
	Block retBlock = mMemblockDevice->readBlock(blocknr);
	
	return retBlock.toString();
}

// Detta är en testfunktion VARNING testfunktion
bool FileSystem::createFolderOn(std::string name, std::string path) {
	return curFolder->addFolder(name, path);
}

bool FileSystem::createFile(std::string name, std::string path)
{
	int block = 1;
	return curFolder->addFile(name, block, path);
}

bool FileSystem::createFolderi(std::string name, std::string path)
{
	return curFolder->addFolder(name, path);
}
