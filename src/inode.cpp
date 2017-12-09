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

inode::inode(std::ifstream&  input, inode* parent)
{
	bool endOfFolderInfo = false;
	std::string output;
	int nrFolders = 0;

	if (parent == nullptr)
	{
		this->parent = this;
	}
	else
	{
		this->parent = parent;
	}

	if (input.is_open()) {
		while (!input.eof() && !endOfFolderInfo) {
			input >> output;
			
			if (output[0] == *"1")
			{
				//1 = this is the current folders name
				output.erase(0, 2);
				this->name = output;
				//TODO: check if problem. root folder has no text. should work
			}
			else if (output[0] == *"2")
			{
				//2 = file name
				output.erase(0, 2);
				this->filesName.push_back(output);
			}
			else if (output[0] == *"3")
			{
				//3 = files position in memory
				output.erase(0, 2);
				this->files.push_back(std::stoi(output));
			}
			else if (output[0] == *"4")
			{
				//4 = folders in this dir
				output.erase(0, 2);
				//TODO: save folder number
				nrFolders = std::stoi(output);
				endOfFolderInfo = true;
			}
			else
			{
				//? = wrong structure of file
				std::cout << "error!!!" << std::endl;
			}
			
		}
	}
	//TODO: go in to first folder recusivly
	for (int i = 0; i < nrFolders; ++i)
	{
		inode toPush = inode(input, this);
		this->folder.push_back(toPush);
	}
}

// Used when adding a file to the system
// Returns whether it could add a file not not.
bool inode::addFile(std::string name, int freeBlock, std::string path) {
	bool fileAdded = false;
    if(path != "")
    {
        inode* addHere = this->goToFolder(path);
		if (addHere != nullptr)
		{
			fileAdded = addHere->addFile(name, freeBlock, "");
		}
        
    }
	else if (name != "") {
		//std::vector<std::string> pathList = pathSplitter(path);

		//inode* folder = findFolderRecursive(pathList, 0, pathList.capacity());

		if (findFile(name) == -1) {
			// Add file.
			filesName.push_back(name);
			files.push_back(freeBlock);

			fileAdded = true;
		}
	}
	return fileAdded;
}

// Used when removing a file from the system.
void inode::removeFile(std::string fileName, std::string path) {
	int id = findFile(fileName);

	if (id != -1) {
		filesName.erase(filesName.begin() + id);
		files.erase(files.begin() + id);
	}
}

// Used to find the ID of a file on the system.
// Return: -1 for not existing. Otherwise returns the value it found.
int inode::findFile(std::string name) const {
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

// Used to add a folder to the system.
// Return: A boolean whether the folder were added or not.
bool inode::addFolder(std::string name, std::string path) {
    //TODO
    //REMOVE NAME
	bool folderAdded = false;
    path = name;
    std::vector<std::string> pathSplit = this->pathSplitter(path);

    
   
    
    if(pathSplit.capacity() > 1)
    {
        inode* addHere = this->findFolderRecursive(pathSplit, 0, pathSplit.capacity()-1);
        if (addHere != nullptr)
        {
            addHere->addFolder(pathSplit.at(pathSplit.capacity()-1), pathSplit.at(pathSplit.capacity()-1));
            folderAdded = true;
        }
    }
	// If no name is entered, name the folder to what the variable NAMELESS_FOLDER is plus the variable i.
	else if (name == "") {
		name = NAMELESS_FOLDER;
		int sizeName = name.size();
		int i = 0;
		while (findFolder(name) >= 0)
		{
			i++;
			name.resize(sizeName);
			name += "(" + std::to_string(i) + ")";
		}
		folderAdded = true;
        
        inode iNode = inode();
        iNode.name = name;
        iNode.parent = this;
        folder.push_back(iNode);
	}
	else if (findFolder(name) == -1) {
		folderAdded = true;
        
        inode iNode = inode();
        iNode.name = name;
        iNode.parent = this;
        folder.push_back(iNode);
	}

	

	return folderAdded;
}

// Used to go to a folder with a path.
// Return: An inode containing the path-folder.
inode* inode::goToFolder(std::string path)  {
	std::vector<std::string> pathList = pathSplitter(path);
	return findFolderRecursive(pathList, 0, pathList.size());
}

// Used to find a folder in the system.
// Return: An integer containing the folder position.
int inode::findFolder(std::string name) const {
	int folderPos = -1;
	for (std::vector<inode>::size_type i = 0; i != folder.size(); i++) {
		if (this->folder[i].getFolderName() == name)
		{
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
	return filesName;
}

std::vector<int> inode::getFilePos() const
{
	return this->files;
}

int inode::getNrOfFolders() const
{
	return this->folder.size();
}

// This function will split any path and returns ? TODO... fix/change?
std::vector<std::string> inode::pathSplitter(std::string path) const {
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

// TODO: memory leak somewhere in this method. vilken Felix inte kan hitta
// This method will find a path recursive to ant path, and return the path.
inode* inode::findFolderRecursive(std::vector<std::string> path, int pos, int cap, bool useWithKnowledge)  {
	
	inode* retINode = nullptr;

	if (useWithKnowledge)
	{
		retINode = this;
	}
	

	if (cap > pos) {
		std::string findFoldername = path.at(pos);

		// If it's a special case (..)
		if (findFoldername == "..") {
			//delete retINode; <--USE?
			retINode = (*parent).findFolderRecursive(path, ++pos, cap, true);
		}

		// If it's a special case (/)
		else if (findFoldername == "") {
			//delete retINode; <--USE?
			retINode = (*getRoot(*this)).findFolderRecursive(path, ++pos, cap, true);
		}
		else {

			// If findFolder is a folder name
			int folderPos = findFolder(findFoldername);
			if (folderPos != -1) {
				retINode = &(folder[folderPos]); //&*(folder.at(folderPos)).findFolderRecursive(path, ++pos, cap);
			}
		}
	}

	return retINode;
}

// This method will return the root of this folder (all folders)
inode* inode::getRoot(inode &current) {
	inode *retInode = &current;

	if (current.parent != &current) {
		retInode = getRoot(*current.parent);
	}

	return retInode;
}



// This function will return a block id of a filename if it finds it.
int inode::findBlockId(std::string fileName) const  {
	int id = findFile(fileName);
	if (id != -1)
		return files[id];
	return -1;
}
