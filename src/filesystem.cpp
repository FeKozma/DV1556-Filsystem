#include "filesystem.h"

//maby not the best thing to use ptr
void FileSystem::createImageRecusive(inode *root, std::string & output)
{
	//TODO: this folderName
	output += "1." + root->getFolderName() + "\n";
	
	//TODO: get filenames
	std::vector<std::string> files = root->getFiles();
	for (int i = 0; i < files.size(); ++i)
	{
		output += "2." + files[i] + "\n";
	}

	//TODO: get position in memory
	std::vector<int> pos = root->getFilePos();
	for (int i = 0; i < pos.size(); ++i)
	{
		output += "3." + std::to_string(pos[i]) + "\n"; 
	}

	//TODO: get all folders
	std::vector<std::string> folder = root->getFolders();
	for (int i = 0; i < folder.size(); ++i)
	{
		output += "4." + folder[i] + "\n";
	}

	output += "\n";
	
	//TODO: go to  folder and recusive
	for (int i = 0; i < folder.size(); ++i)
	{
		createImageRecusive(root->goToFolder(folder[i]), output);
	}
}

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

bool FileSystem::createImage(std::string filename, std::string path)
{
	std::string stringToFile = "";
	//TODO: go to root
	inode* rootFolder = this->curFolder->goToFolder("/");
	createImageRecusive(rootFolder, stringToFile);
	//TODO: save string to file
	std::cout << std::endl << stringToFile << std::endl;
	std::ofstream out(filename + ".txt");
	out << stringToFile;
	out.close();

	return true;
}

bool FileSystem::loadImage(std::string filename)
{
	//TODO: open file
	//TODO: if no file is found return false
	//TODO: create new inode with suitible constructur
	return false;
}

/****************************************************
 ***************** Begin Test? Functions ******************
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

// This method removes spaces at the end of a string.
// Is using the includes <algorithm>.
void FileSystem::stringTrim(std::string &s) {
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
}

// This function will create and add a new file to the system.
// Returns: A boolean wether the folder were created or not.
bool FileSystem::createFile(std::string content, std::string name, std::string path) {
	if (name == "") return false; 

	bool fileCreated = false;
	int freeBlock = -1;
	
	//insert file into free spot
	for (int i = 0; i < nrOfBlocks; ++i) {
		if (availableBlocks[i] == true) {
			createFileOn(content, i);
			freeBlock = i;

			// Prepare size for the memBlockDevice, what it needs to be as long as a block.
			for (int k = content.size(); k < 512; ++k) {
				content += " ";
			}


			i = nrOfBlocks;
		}
	}
    
    //check if path exists and if so, mark the memory pos as taken
    if (curFolder->addFile(name, freeBlock, path))
    {
        if (this->mMemblockDevice->writeBlock(freeBlock, content) == 1)
		{
            fileCreated = true;
			availableBlocks[freeBlock] = false;
        }
    }
    
    
	return fileCreated;
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
	if (path != "") {
		inode *folder = curFolder->goToFolder(path);
		if (folder != nullptr)
		{ 
			curFolder = folder;
		}
	}

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
int FileSystem::formatSystem() {
	int deletedFiles = 0;
	for (int i = 0; i < nrOfBlocks; ++i) {
		if (availableBlocks[i] == false)
		{
			availableBlocks[i] = true;
			deletedFiles += 1;
		}
	}
	delete curFolder;
	curFolder = new inode();
	return deletedFiles;
}
