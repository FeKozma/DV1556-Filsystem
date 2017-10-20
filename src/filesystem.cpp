#include "filesystem.h"

FileSystem::FileSystem(int blockSize) {
	mMemblockDevice = new MemBlockDevice(blockSize);
	availableBlocks = new bool[blockSize];
	for (int i = 0; i < blockSize; ++i)
		availableBlocks[i] = true;
	curFolder = new inode();
	nrOfBlocks = blockSize;
}

FileSystem::~FileSystem() {
	delete mMemblockDevice;
	delete curFolder;
	delete[] availableBlocks;
}

// This is a test function, WARNING, TODO
int FileSystem::createFileOn(std::string storeString, int blocknr) {
	int lengthOfBlock = 512;

	for (int i = storeString.length(); i < lengthOfBlock; i++) {
		storeString += "0";
	}
	//std::cout << std::to_string(storeString.length()) + "\n";
	int retVal = this->mMemblockDevice->writeBlock(blocknr, storeString);
	return retVal;
}

// This is a test function, WARNING, TODO
std::string FileSystem::viewFileOn(int blocknr) {
	Block retBlock = mMemblockDevice->readBlock(blocknr);
	
	return retBlock.toString();
}

bool FileSystem::createFile(std::string data, std::string name, std::string path) {
	if (path == "") path = curFolder->getFolderPath();

	bool fileCreated = false;
	int freeBlock = -1;

	for (int i = 0; i < nrOfBlocks; ++i) {
		if (availableBlocks[i] == true) {
			createFileOn(data, i);
			availableBlocks[i] = false;
			fileCreated = true;
			freeBlock = i;
			break;
		}
	}

	return fileCreated ? curFolder->addFile(name, freeBlock, path) : false;
}

std::string FileSystem::createFolderi(std::string name, std::string path) {
	if (name == "") return "Error: No name entered.";
	if (path == "") path = curFolder->getFolderPath();

	curFolder->addFolder(name, path);
	return path + name;
}

std::string FileSystem::goToFolder(std::string path) {
	//TODO: return an error message saying the folder doesn't exist.
	if (path != "") curFolder = curFolder->goToFolder(path);

	return curFolder->getFolderPath();
}

std::string FileSystem::getCurrentPath() {
	return curFolder->getFolderPath();
}

std::string FileSystem::listDir(std::string path) {
	std::vector<std::string> folders = curFolder->getFolders();
	std::vector<std::string> files = curFolder->getFiles();
	std::string retString = "Type\tName\tPermission\tSize\n";

	// TODO: complete the string.
	for (int i = 0; i < folders.size(); ++i) {
		retString += "DIR\t" + folders[i] + "\n";
	}
	for (int i = 0; i < files.size(); ++i) {
		retString += "FILE\t" + files[i] + "\n";
	}

	return retString;
}