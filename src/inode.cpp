#include "inode.h"
#include <iostream>


const std::string NAMELESS_FOLDER = "no-name"; // Used for folders without a name.

// Used when initializing root.
inode::inode() {
	this->parent = this;
}

// Used when initializing folders.
inode::inode(inode* &parent) {
	this->parent = parent;
}

inode::inode(std::ifstream&  input, inode* parent) {
	bool endOfFolderInfo = false;
	std::string output;
	int nrFolders = 0;

	if (parent == nullptr) {
		this->parent = this;
	}
	else {
		this->parent = parent;
	}

	if (input.is_open()) {
		while (!input.eof() && !endOfFolderInfo) {
			input >> output;
			
			if (output[0] == *"1") {
				//1 = this is the current folders name
				output.erase(0, 2);
				this->name = output;
			}
			else if (output[0] == *"2") {
				//2 = file name
				output.erase(0, 2);
				this->filesName.push_back(output);
			}
			else if (output[0] == *"3") {
				//3 = files position in memory
				output.erase(0, 2);
				this->files.push_back(std::stoi(output));
			}
			else if (output[0] == *"4") {
				//4 = folders in this dir
				output.erase(0, 2);

				nrFolders = std::stoi(output);
				endOfFolderInfo = true;
			}
			else {
				std::cout << "An unknown error was found!" << std::endl;
			}
		}
	}
	// Go in to first folder recusivly.
	for (int i = 0; i < nrFolders; ++i) {
		inode toPush = inode(input, this);
		this->folder.push_back(toPush);
	}
}

/** Used when adding a file to the system
	* Returns whether it could add a file not not.
	*/
bool inode::addFile(const std::string & name,const int freeBlock,const std::string& path) {
	bool fileAdded = false;
	if(path != "") {
		inode* addHere = this->goToFolder(path);
		if (addHere != nullptr) {
			fileAdded = addHere->addFile(name, freeBlock, "");
		}
	}
	else if (name != "") {
		if (findFile(name) == -1) {
			// Add file.
			filesName.push_back(name);
			files.push_back(freeBlock);

			fileAdded = true;
		}
	}
	return fileAdded;
}

bool inode::addFile(const std::string & path,const int freeBlock) {
	std::string* arr = this->getPathAndFileName(path);
	return addFile(arr[0], freeBlock, arr[1]);
}

// Used when removing a file from the system.
void inode::removeFile(const std::string& fileName, const std::string& path) {
	int id = findFile(fileName);

	if (id != -1) {
		filesName.erase(filesName.begin() + id);
		files.erase(files.begin() + id);
	}
}

/** Used to find the ID of a file on the system.
 * Return: -1 for not existing. Otherwise returns the value it found.
 */
int inode::findFile(const std::string& name) const {
	int filePos = -1;
	for (int i = 0; i <  filesName.size(); ++i) {
		if (filesName[i] == name)
		{
			filePos = i;
			break;
		}
	}
	return filePos;
}

/** Used to add a folder to the system.
 * Return: A boolean whether the folder were added or not.
 */
bool inode::addFolder(std::string path) {
	bool folderAdded = false;
	//path = name;
	std::vector<std::string> pathSplit = this->pathSplitter(path);
	
	if(pathSplit.capacity() > 1)
	{
		inode* addHere = this->findFolderRecursive(pathSplit, 0, pathSplit.capacity()-1);
		if (addHere != nullptr)
		{
			addHere->addFolder(pathSplit.at(pathSplit.capacity()-1));
			folderAdded = true;
		}
	}
	// If no name is entered, name the folder to what the variable NAMELESS_FOLDER is plus the variable i.
	else if (path == "") {
		path = NAMELESS_FOLDER;
		int sizeName = path.size();
		int i = 0;
		while (findFolder(path) >= 0)
		{
			i++;
			path.resize(sizeName);
			path += "(" + std::to_string(i) + ")";
		}
		folderAdded = true;

		inode iNode = inode();
		iNode.name = path;
		iNode.parent = this;
		folder.push_back(iNode);
	}
	else if (findFolder(path) == -1) {
		folderAdded = true;

		inode iNode = inode();
		iNode.name = path;
		iNode.parent = this;
		folder.push_back(iNode);
	}

	return folderAdded;
}

/** Used to go to a folder with a path.
 * Return: An inode containing the path-folder.
 */
inode* inode::goToFolder(const std::string &path) {
	std::vector<std::string> pathList = pathSplitter(path);
	if (path == "") {
		return this;
	}
	else {
		return findFolderRecursive(pathList, 0, pathList.size());
	}
}

/** Used to find a folder in the system.
/* Return: An integer containing the folder position.
*/
int inode::findFolder(std::string name) const {
	int folderPos = -1;
	for (std::vector<inode>::size_type i = 0; i != folder.size(); i++) {
		if (this->folder[i].getFolderName() == name) {
			folderPos = i;
		}
	}
	return folderPos;
}

// Returns the folder name.
std::string inode::getFolderName() const {
	return this->name;
}

// Returns the current folder path of this inode.
std::string inode::getFolderPath() const {
	std::string path = "/";
	const inode *current = this;
	while (current->getFolderName() != "") {
		path = "/" + current->getFolderName() + path;
		current = current->parent;
	}

	return path;
}

// Returns an array of all folders in this inode.
std::vector<std::string> inode::getFolders() const {
	std::vector<std::string> folders;
	for (int i = 0; i < folder.size(); ++i) {
		folders.push_back(folder[i].getFolderName());
	}
	return folders;
}

// Returns the files.
std::vector<std::string> inode::getFiles() const {
	return this->filesName;
}

std::vector<int> inode::getFilePos() const {
	return this->files;
}

int inode::getMemPosGivenPosInArr(const int & pos) const {
	if (pos >= 0 && pos < files.size())
		return this->files[pos];
	else
		return -1;
}

int inode::getNrOfFolders() const {
	return this->folder.size();
}

// This function will split any path and returns.
std::vector<std::string> inode::pathSplitter(const std::string path) const {
	std::vector<std::string> retPath;

	int start = 0;

	for (int i = 0; i < path.length(); ++i) {
		if (path[i] == '/') {
			retPath.push_back(path.substr(start, i - start));
			start = i + 1;
		}
	}

	// If the path doesn't end with /, add the last path.
	if (start != path.length()) {
		retPath.push_back(path.substr(start));
	}

	return retPath;
}

// This method will find a path recursive to ant path, and return the path.
inode* inode::findFolderRecursive(const std::vector<std::string> & path, const int pos, const int cap, const bool useWithKnowledge) {

	inode* retINode = nullptr;

	if (useWithKnowledge) {
		retINode = this;
	}


	if (cap > pos) {
		std::string findFoldername = path.at(pos); 

		// If it's a special case (..)
		if (findFoldername == "..") {
			//delete retINode; <--USE?
			retINode = (*parent).findFolderRecursive(path, pos + 1, cap, true);
		}

		// If it's a special case (/)
		else if (findFoldername == "") {
			//delete retINode; <--USE?
			retINode = (*getRoot(*this)).findFolderRecursive(path, pos + 1, cap, true);
		}
		else {

			// If findFolder is a folder name
			int folderPos = findFolder(findFoldername);
			if (folderPos != -1) {
				if (cap > pos + 1)
				{
					inode* test = &*(folder.at(folderPos)).findFolderRecursive(path, pos + 1, cap);
					retINode = test;
				}
				else  {
					retINode = &(folder[folderPos]);

				}
			
			}
		}
	}

	return retINode;
}

inode * inode::findFolderContainingFileRecursive(const std::string &path) {
	std::vector<std::string> pathList = this->pathSplitter(path);
	if (pathList.size() == 1) {
		return this;
	}
	else {
		return findFolderRecursive(pathList, 0, pathList.size() - 1);
	}
}

// This method will return the root of this folder (all folders)
inode* inode::getRoot(inode &current) {
	inode *retInode = &current;

	if (current.parent != &current) {
		retInode = getRoot(*current.parent);
	}

	return retInode;
}

std::string* inode::getPathAndFileName(const std::string &path) const {
	int start = 0;
	for (int i = path.size() - 1; i >= 0; --i) {
		if (path[i] == '/') {
			start = i + 1;
			i = 0;
		}
	}

	return new std::string[2] { path.substr(0, start), path.substr(start) };
}

bool inode::renameFileGivenName(const std::string &oldFile, const std::string &newFile) {
	bool retVal = false;
	for (int i = 0; i < this->files.size(); ++i) {
		if (filesName[i] == oldFile) {
			filesName[i] = newFile;
			retVal = true;
		}
	}
	return retVal;
}


// This function will return a block id of a filename if it finds it.
int inode::findBlockIdPath(const std::string& pathName)   {
	inode* path = this->findFolderContainingFileRecursive(pathName);

	if (path == nullptr || path == this) {
		return findBlockId(pathName);
	}
	else {
		return path->findBlockId(getLast(pathName));
	}

	
}

int inode::findBlockId(std::string pathName) {
	int id = findFile(getLast(pathName));
	if (id != -1)
		return files[id];
	return -1;
}

std::string inode::getLast(std::string path) {
	std::size_t found = path.find_last_of("/\\");
	return path.substr(found + 1);
}

std::string inode::ignoreLast(const std::string & path) const {
	std::size_t found = path.find_last_of("/");
	if (found > path.size())
	{
		found = -1;
	}
	return path.substr(0, found+1);
}

std::string inode::listDir() {
	std::string retStr = "Listing directory\nType\t\tName\t\tPermission\tSize\n";
	std::vector<std::string> folderNames = getFolders();

	for (int i = 0; i < folderNames.size(); ++i) {
		retStr += "DIR\t\t" + folderNames[i] + (folderNames[i].length() <= 8 ? "\t" : "") + "\trw\t\tNULL byte\n";
	}
	for (int i = 0; i < filesName.size(); ++i) {
		retStr += "FILE\t\t" + filesName[i] + (filesName[i].length() <= 8 ? "\t" : "") + "\trw\t\tNULL byte\n";
	}

	return retStr;
}

bool inode::renameFileGivenPath(std::string oldFile, std::string newFile) {
	bool retVal = false;
	inode* folder = this->findFolderContainingFileRecursive(oldFile);

	if (folder->findBlockId(newFile) == -1) {
		if (newFile != "") {
			if (newFile.find_first_of('/') == std::string::npos) {
				retVal = folder->renameFileGivenName(folder->getLast(oldFile), newFile);
			}
		}
	}

	return retVal;
}

bool inode::updatePos(const std::string & file, const int newPos) {
	bool updated = false;
	for (int i = 0; i < filesName.size(); ++i)  {
		if (filesName[i] == file) {
			this->files[i] = newPos;
			i = filesName.size();
			updated = true;
		}
	}

	return updated;
}
