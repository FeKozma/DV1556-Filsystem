#include "filesystem.h"


FileSystem::FileSystem(int blockSize, int fileSize) {
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

/****************************************************
 ***************** Begin Functions ******************
 ****************** WARNING TODO ********************
 ***************************************************/

int FileSystem::createFileOn(std::string storeString, int blocknr) {
	int lengthOfBlock = fileSize;
	
	for (int i = storeString.length(); i < lengthOfBlock; i++) {
		storeString += "0";
	}
	//std::cout << std::to_string(storeString.length()) + "\n";
	int retVal = this->mMemblockDevice->writeBlock(blocknr, storeString);
	return retVal;
}

/****************************************************
 **************** END TEST FUNCTIONS ****************
 ***************************************************/

// This function will return a string with the content of the fileName.
std::string FileSystem::viewFileOn(std::string fileName) {
	std::string path = curFolder->getFolderPath();

	int blockId = curFolder->findBlockId(fileName);
	
	std::string content = "";

	if (blockId != -1) {
		Block block = mMemblockDevice->readBlock(blockId);
		
		for (int i = 0; i < block.size(); ++i) {
			content += block[i];
		}
	}
	stringTrim(content);
	return content;
}

void FileSystem::stringTrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// This function will create a file to the system.
// Returns: A boolean wether the folder were created or not.
bool FileSystem::createFile(std::string conntent, std::string name, std::string path) {
	if (path == "") path = curFolder->getFolderPath();
	if (name == "" || conntent == "") return false;

	bool fileCreated = false;
	int freeBlock = -1;

	for (int i = 0; i < nrOfBlocks; ++i) {
		if (availableBlocks[i] == true) {
			createFileOn(conntent, i);
			availableBlocks[i] = false;
			fileCreated = true;
			freeBlock = i;

			//preper size for the memblockDevice needs to be ass long as a block
			for (int k = conntent.size(); k < 512; ++k)
			{
				conntent += " ";
			}

			break;
		}
	}

	return fileCreated ? (curFolder->addFile(name, freeBlock, path) && (1 == this->mMemblockDevice->writeBlock(freeBlock, conntent))) : false;
}

// This function will create a folder.
// Returns: current folder's path + name.
std::string FileSystem::createFolderi(std::string name, std::string path) {
	if (name == "") return "Error: No name entered.";
	if (path == "") path = curFolder->getFolderPath();

	curFolder->addFolder(name, path);
	return path + name;
}

// This folder will remove a file from the system.
void FileSystem::removeFile(std::string fileName) {
	std::string path = curFolder->getFolderPath();

	int blockId = curFolder->findBlockId(fileName);
	
	if (blockId != -1) {
		availableBlocks[blockId] = true;
		curFolder->removeFile(fileName);
	}
}

// This function will go to a folder in the system.
// Returns: It will return the folder it got to.
std::string FileSystem::goToFolder(std::string path) {
	// TODO: return an error message saying the folder doesn't exist.
	if (path != "") curFolder = curFolder->goToFolder(path);

	return curFolder->getFolderPath();
}

// This function will return the current folder path.
std::string FileSystem::getCurrentPath() {
	return curFolder->getFolderPath();
}

// This function will return the current list of a directory.
std::string FileSystem::listDir(std::string path) {
	std::vector<std::string> folders = curFolder->getFolders();
	std::vector<std::string> files = curFolder->getFiles();
	std::string retString = "Listing directory\nType\t\tName\t\tPermission\tSize\n";

	// TODO: complete the string.
	for (int i = 0; i < folders.size(); ++i) {
		retString += "DIR\t\t" + folders[i] + (folders[i].length() <= 8 ? "\t" : "") + "\trw\t\t0 byte\n";
	}
	for (int i = 0; i < files.size(); ++i) {
		retString += "FILE\t\t" + files[i] + (files[i].length() <= 8 ? "\t" : "") + "\trw\t\t0 byte\n";
	}

	return retString;
}

// This function will format the system (our current filesystem).
void FileSystem::formatSystem() {
	for (int i = 0; i < nrOfBlocks; ++i) {
		availableBlocks[i] = true;
	}
	delete curFolder;
	curFolder = new inode();
}
