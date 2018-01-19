#include "filesystem.h"

FileSystem::FileSystem(const int &blockSize, const int &fileSize) {
	mMemblockDevice = new MemBlockDevice(blockSize);
	curFolder = new inode();
	this->fileSize = fileSize;
}

FileSystem::~FileSystem() {
	delete mMemblockDevice;
	delete curFolder;
}

void FileSystem::createImageRecursive(inode *root, std::string &output) {
	// This folder's name
	output += "1." + root->getFolderName() + "\n";

	// Get filenames
	std::vector<std::string> files = root->getFiles();
	for (int i = 0; i < files.size(); ++i) {
		output += "2." + files[i] + "\n";
	}

	// Get position in memory
	std::vector<int> pos = root->getFilePos();
	for (int i = 0; i < pos.size(); ++i) {
		output += "3." + std::to_string(pos[i]) + "\n";
	}

	// Get nr of folders
	std::vector<std::string> folder = root->getFolders();

	output += "4." + std::to_string(root->getNrOfFolders()) + "\n";

	output += "\n";

	// Go to folder and go recursively.
	for (int i = 0; i < folder.size(); ++i) {
		createImageRecursive(root->goToFolder(folder[i]), output);
	}
}

bool FileSystem::createImage(std::string filename) {
	// If no filename is entered, set it to the default filename.
	if (filename == "") {
		filename = "image";
	}

	std::string stringToFile = "";

	// Go to root
	inode* rootFolder = this->curFolder->goToFolder("/");
	createImageRecursive(rootFolder, stringToFile);

	stringToFile += mMemblockDevice->filesImage();

	// Save string to file.
	std::cout << std::endl << stringToFile << std::endl;
	std::ofstream out(filename + ".txt");
	out << stringToFile;
	out.close();

	return true;
}

bool FileSystem::loadImage(std::string filename) {
	bool loaded = false;

	// If no filename is entered, set it to the default filename.
	if (filename == "") {
		filename = "image";
	}

	// Open file
	std::ifstream input;
	input.open(filename + ".txt");

	if (input.is_open()) {
		// Create new inode with suitable constructur.
		delete this->curFolder;
		this->curFolder = new inode(input);

		// Load mMemblockDevice
		this->mMemblockDevice->readFilesImage(input);

		input.close();

		loaded = true;
	}

	return loaded;
}
int FileSystem::createFileOn(std::string storeString) {
	int lengthOfBlock = fileSize -1;
	
	// "+ 1" beacuse it store if the block is continuing inside the block
	for (int i = storeString.length(); 0 != (i % lengthOfBlock); i++) {
		storeString += " ";
	}

	int nrOfBlocks = storeString.size() / lengthOfBlock;

	//std::cout << std::to_string(storeString.length()) + "\n";
	int retVal = this->mMemblockDevice->writeBlock(storeString, nrOfBlocks);
	return retVal;
}

// This function will return a string with the content of the fileName in path.
std::string FileSystem::viewFileOn(const std::string &fileName) const {
	//std::string path = curFolder->getFolderPath();

	int blockId = curFolder->findBlockIdPath(fileName); //-1
	
	return viewFileOn(blockId);
}

// This function will return a string with the content of the file on pos.
std::string FileSystem::viewFileOn(int pos) const {
	//std::string path = curFolder->getFolderPath();

	pos--;
	std::string content = "";
	Block block;
	if (pos != -2) {
		do {
			pos++;
			block = mMemblockDevice->readBlock(pos);

			for (int i = 1; i < block.size(); ++i) {
				content += block[i];
			}
		} while (block.getIfMore());
	}
	stringTrim(content);
	return content;
}

/**
 * This method removes spaces at the end of a string.
 * Is using the includes <algorithm>.
 */
std::string FileSystem::stringTrim(std::string &k) const {
	std::string s = k;
	s.erase(std::find_if(s.rbegin(), s.rend(), [](int ch) {
		return !std::isspace(ch);
	}).base(), s.end());
	return s;
}

/** This function will create and add a new file to the system.
 * Returns: A boolean wether the folder were created or not.
 */
bool FileSystem::createFile(const std::string &content, const std::string &path) {
	std::string name = this->getAfterLastSlash(path);
	if (name == "") {
		return false;
	}

	bool fileCreated = false;
	int freeBlock = -1;
	
	int space = this->mMemblockDevice->spaceLeft();

	// Check if path exists, if so, mark the memory position as taken.
	if (space != 0) {
 		int pos = createFileOn(content);

		if (curFolder->addFile(name, pos, this->ignoreLastSlash(path))) {
			fileCreated = true;
		}
		else {
			this->mMemblockDevice->rmBlock(pos);
		}
	}

	return fileCreated;
}

/** This function will create a folder.
 * Returns: current folder's path + name.
 */
bool FileSystem::createFolderi(std::string path) {
	if (path == "") path = curFolder->getFolderPath();

	return curFolder->addFolder(path);
}

// This folder will remove a file from the system.
bool FileSystem::removeFile(const std::string &path) {
	bool retVal = false;
	inode* folder = curFolder->goToFolder(ignoreLastSlash(path));
	if (folder != nullptr) {
		std::string fileName = getAfterLastSlash(path);
		int blockId = folder->findBlockId(path);

		if (this->mMemblockDevice->rmBlock(blockId)) {
			folder->removeFile(fileName);
			retVal = true;
		}
	}
	return retVal;
}

// This function will go to a folder in the system.
// Returns: It will return the folder it got to.
std::string FileSystem::goToFolder(const std::string &path) {
	if (path != "") {
		inode *folder = curFolder->goToFolder(path);
		if (folder != nullptr) {
			curFolder = folder;
		}
	}

	return curFolder->getFolderPath();
}

// This function will return the current folder path.
std::string FileSystem::getCurrentPath() const {
	return curFolder->getFolderPath();
}

// This function will return the current list of a directory.
std::string FileSystem::listDir(std::string path) {
	inode* folder = this->curFolder->goToFolder(path);
	if (folder == nullptr) {
		return "No path.\n";
	}
	/*
	return folder->listDir();
	*/
	std::vector<std::string> folderNames = folder->getFolders();
	std::vector<std::string> filesName = folder->getFiles();
	std::vector<int> filesPos = folder->getFilePos();

	std::string retStr = "Listing directory\nType\t\tName\t\tPermission\tSize\n";

	for (int i = 0; i < folderNames.size(); ++i) {
		retStr += "DIR\t\t" + folderNames[i] + (folderNames[i].length() <= 8 ? "\t" : "") + "\trw\n";
	}
	for (int i = 0; i < filesName.size(); ++i) {
		retStr += "FILE\t\t" + filesName[i] + (filesName[i].length() <= 8 ? "\t" : "") + "\trw\t\t" + std::to_string(this->fileSize * this->mMemblockDevice->lengthOfFile(filesPos[i])) + " byte\n";
	}

	return  retStr;
}

// This function will return the current list of this directory.
std::string FileSystem::listDir() {
	return listDir("");
}

// This function will format the system (our current filesystem).
int FileSystem::formatSystem() {
	int deletedFiles = mMemblockDevice->formatSys();

	delete curFolder;
	curFolder = new inode();
	return deletedFiles;
}

bool FileSystem::copyFile(const std::string &oldFile, std::string newFile) {
	// Check if newFile exists.
	bool success = false;

	if (newFile[newFile.size() - 1] == '/') {
		newFile += this->getAfterLastSlash(oldFile);
	}
	else {
		newFile += '/' + this->getAfterLastSlash(oldFile);
	}

	int pos = this->curFolder->findBlockIdPath(oldFile);
	if (pos != -1) {
		inode* addFileHere = this->curFolder->goToFolder(ignoreLastSlash(newFile));
		if (addFileHere != nullptr) {
			int newPos = this->mMemblockDevice->copyBlock(pos);
			success = true;
			if (!addFileHere->addFile(this->getAfterLastSlash(newFile), newPos, "")) {
				this->mMemblockDevice->rmBlock(newPos);
				success = false;
			}
		}
	}
	return success;
}

bool FileSystem::renameFileGivenPath(const std::string &oldFile, const std::string &newFile) {
	return this->curFolder->renameFileGivenPath(oldFile, newFile);
}

std::string FileSystem::ignoreLastSlash(const std::string &path) const {
	std::size_t found = path.find_last_of("/");
	if (found > path.size()) {
		found = -1;
	}
	return path.substr(0, found + 1);
}

std::string FileSystem::getAfterLastSlash(const std::string &path) const {
	std::size_t found = path.find_last_of("/\\");
	return path.substr(found + 1);
}

std::string FileSystem::getDiskAllocations() const {
	return this->mMemblockDevice->getDiskAllocations();
}

bool FileSystem::appendFile(const std::string &file1, const std::string &file2) {
	bool retVal = false;
	inode* folder1 = this->curFolder->goToFolder(this->ignoreLastSlash(file1));
	inode* folder2 = this->curFolder->goToFolder(this->ignoreLastSlash(file2));

	// Check if files exists.
	if (folder1 != nullptr && folder2 != nullptr) {
		int posFile1 = folder1->getMemPosGivenPosInArr(folder1->findFile(this->getAfterLastSlash(file1)));
		int posFile2 = folder2->getMemPosGivenPosInArr(folder2->findFile(this->getAfterLastSlash(file2)));
		if (posFile1 != -1 && posFile2 != -1) {

			// Get amount of text
			std::string content = this->viewFileOn(posFile2);
			content = this->stringTrim(content);
			content += this->viewFileOn(posFile1);
			content = this->stringTrim(content);

			// Temporarliy remove posFile2
			this->mMemblockDevice->rmBlock(posFile2);

			// Check if space exists
			if (this->mMemblockDevice->findFree(1 + (content.size() / (fileSize - 1))) != -1) {
				int newPos = this->createFileOn(content);
				folder2->updatePos(this->ignoreLastSlash(file2), newPos);
				retVal = true;
			}
			else {
				this->mMemblockDevice->addBlock(posFile2);
			}
		}
	}

	return retVal;
}
