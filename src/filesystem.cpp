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

	//TODO: get nr of folders
	std::vector<std::string> folder = root->getFolders();

	
	output += "4." + std::to_string(root->getNrOfFolders()) + "\n";
	

	output += "\n";
	
	//TODO: go to  folder and recusive
	for (int i = 0; i < folder.size(); ++i)
	{
		createImageRecusive(root->goToFolder(folder[i]), output);
	}
}

FileSystem::FileSystem(int blockSize, int fileSize) {
	mMemblockDevice = new MemBlockDevice(blockSize);
	//availableBlocks = new bool[blockSize];
	//for (int i = 0; i < blockSize; ++i)
	//	availableBlocks[i] = true;
	curFolder = new inode();
	//nrOfBlocks = blockSize;
	this->fileSize = fileSize;
}

FileSystem::~FileSystem() {
	delete mMemblockDevice;
	delete curFolder;
	//delete[] availableBlocks;
}

bool FileSystem::createImage(std::string filename, std::string path)
{
	std::string stringToFile = "";
	//TODO: go to root
	inode* rootFolder = this->curFolder->goToFolder("/");
	createImageRecusive(rootFolder, stringToFile);

	stringToFile += mMemblockDevice->filesImage();
	//TODO: save string to file
	std::cout << std::endl << stringToFile << std::endl;
	std::ofstream out(filename + ".txt");
	out << stringToFile;
	out.close();

	return true;
}

bool FileSystem::loadImage(std::string filename)
{
	bool added = false;
	// open file
	std::ifstream input;
	input.open(filename + ".txt");

	if (input.is_open()) {
		// create new inode with suitible constructur
		delete this->curFolder;
		this->curFolder = new inode(input);
		
		//TODO: load mMemblockDevice
		this->mMemblockDevice->readFilesImage(input);
		
		input.close();

		added = true;
	}

	return added;
}



/****************************************************
 ***************** Begin Test? Functions ******************
 ****************** WARNING TODO ********************
 ***************************************************/

int FileSystem::createFileOn(std::string storeString) {
	int lengthOfBlock = fileSize -1;
	
	// "+ 1" beacuse i store if the block is continuing inside the block
	for (int i = storeString.length(); 0 != (i % lengthOfBlock); i++) {
		storeString += " ";
	}

	int nrOfBlocks = storeString.size() / lengthOfBlock;

	//std::cout << std::to_string(storeString.length()) + "\n";
	int retVal = this->mMemblockDevice->writeBlock(storeString, nrOfBlocks);
	return retVal;
}

/****************************************************
 **************** END TEST FUNCTIONS ****************
 ***************************************************/

// This function will return a string with the content of the fileName.
std::string FileSystem::viewFileOn(std::string fileName) {
	//std::string path = curFolder->getFolderPath();

	int blockId = curFolder->findBlockIdPath(fileName) - 1;
	
	std::string content = "";
	Block block;
	if (blockId != -2) {
		do {
			blockId++;
			block = mMemblockDevice->readBlock(blockId);

			for (int i = 1; i < block.size(); ++i) {
				content += block[i];
			}
		} while (block.getIfMore());
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

	std::size_t found = name.find_first_of("/\\");
	if (found < name.size() || name == "")
	{
		return false;
	}

	bool fileCreated = false;
	int freeBlock = -1;
	
	int space = this->mMemblockDevice->spaceLeft();

    
    //check if path exists and if so, mark the memory pos as taken
    if (space != 0)
    {
 		int pos = createFileOn(content);

		if (curFolder->addFile(name, pos, path))
		{
			fileCreated = true;
		}
		else
		{
			this->mMemblockDevice->rmBlock(pos);
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
bool FileSystem::removeFile(std::string path) {
	
	bool retVal = false;
	inode* folder = curFolder->goToFolder(ignoreLast(path));
	if (folder != nullptr)
	{
		std::string fileName = getLast(path);
		int blockId = folder->findBlockId(path);

		if (this->mMemblockDevice->rmBlock(blockId))
		{
			folder->removeFile(fileName);
			retVal = true;
		}
	}
	return retVal;
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
std::string FileSystem::listDir(std::string path)
{
	inode* folder = this->curFolder->goToFolder(path);
	if (folder == nullptr)
	{
		folder = this->curFolder;
	}
	return folder->listDir();
	
}

// This function will return the current list of this dir.
std::string FileSystem::listDir() {
	std::vector<std::string> folders = curFolder->getFolders();
	std::vector<std::string> files = curFolder->getFiles();
	std::string retString = "Listing directory\nType\t\tName\t\tPermission\tSize\n";

	// TODO: complete the string.
	for (int i = 0; i < folders.size(); ++i) {
		retString += "DIR\t\t" + folders[i] + (folders[i].length() <= 8 ? "\t" : "") + "\trw\t\t? byte\n";
	}
	for (int i = 0; i < files.size(); ++i) {
		retString += "FILE\t\t" + files[i] + (files[i].length() <= 8 ? "\t" : "") + "\trw\t\t? byte\n";
	}

	return retString;
}

// This function will format the system (our current filesystem).
int FileSystem::formatSystem() {
	int deletedFiles = mMemblockDevice->formatSys();

	delete curFolder;
	curFolder = new inode();
	return deletedFiles;
}

bool FileSystem::copyFile(std::string oldFile, std::string newFile)
{
	// check if newFile exists
	bool succes = false;

	int pos = this->curFolder->findBlockIdPath(oldFile);
	if (pos != -1)
	{
		inode* addFileHere = this->curFolder->goToFolder(ignoreLast(newFile));
		if (addFileHere != nullptr)
		{
			int newPos = this->mMemblockDevice->copyBlock(pos);
			succes = true;
			if (!addFileHere->addFile(this->getLast(newFile), newPos, ""))
			{
				this->mMemblockDevice->rmBlock(newPos);
				succes = false;
			}

		}
	}
	return succes;

}

bool FileSystem::renameFileGivenPath(std::string oldFile, std::string newFile)
{
	return this->curFolder->renameFileGivenPath(oldFile, newFile);
}

std::string FileSystem::ignoreLast(const std::string & path) const
{
	std::size_t found = path.find_last_of("/");
	if (found > path.size())
	{
		found = -1;
	}
	return path.substr(0, found + 1);
}

std::string FileSystem::getLast(const std::string & path) const
{
	std::size_t found = path.find_last_of("/\\");
	return path.substr(found + 1);
}

std::string FileSystem::getDiskAllocations()
{
	return this->mMemblockDevice->getDiskAllocations();
}


