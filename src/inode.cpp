#include "inode.h"


const std::string NAMELESS_FOLDER = "no-name"; // Used for folders without a name.

// Used when initializing root.
inode::inode() {
	this->parent = this;
}

// Used when initializing folders.
inode::inode(inode* &parent) {
	this->parent = parent;
}

// Used when adding a file to the system
// Returns whether it could add a file not not.
bool inode::addFile(std::string name, int freeBlock, std::string path) {
	bool fileAdded = false;
    if(path != "")
    {
        inode* addHere = this->goToFolder(path);
        addHere->addFile(name, freeBlock, "");
        
    }
	else if (name != "") {
		std::vector<std::string> pathList = pathSplitter(path);

		inode* folder = findFolderRecursive(pathList, 0, pathList.capacity());

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
int inode::findFile(std::string name) {
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
	bool folderAdded = false;
    //TODO
	//REMOVE NAME!!
	path = name;
    std::vector<std::string> pathSplit = this->pathSplitter(path);

    
   
    
    if(pathSplit.capacity() > 1)
    {
        inode addHere = this->findFolderRecursive(pathSplit, 0, pathSplit.capacity()-1);
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
	}
	else if (findFolder(name) == -1) {
		folderAdded = true;
	}

	if (folderAdded) {
		// Add folder
		inode iNode = inode();
		iNode.name = name;
		iNode.parent = goToFolder(path);
		folder.push_back(iNode);
	}

	return folderAdded;
}

// Used to go to a folder with a path.
// Return: An inode containing the path-folder.
inode* inode::goToFolder(std::string path) {
	std::vector<std::string> pathList = pathSplitter(path);
	return findFolderRecursive(pathList, 0, pathList.size());
}

// Used to find a folder in the system.
// Return: An integer containing the folder position.
int inode::findFolder(std::string name) {
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
std::string inode::getFolderName() {
	return this->name;
}

// Returns the current folder path of this inode.
std::string inode::getFolderPath() {
	std::string path = "/";
	inode *current = this;
	while (current->getFolderName() != "") {
		path = "/" + current->getFolderName() + path;
		current = current->parent;
	}

	return path;
}

// Returns an array of all folders in this inode.
std::vector<std::string> inode::getFolders() {
	std::vector<std::string> folders;
	for (int i = 0; i < folder.size(); ++i) {
		folders.push_back(folder[i].getFolderName());
	}
	return folders;
}

// Returns the files.
std::vector<std::string> inode::getFiles() {
	return filesName;
}

// This function will split any path and returns ? TODO... fix/change?
std::vector<std::string> inode::pathSplitter(std::string path) {
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

// TODO: memory leak somewhere in this method.
// This method will find a path recursive to ant path, and return the path.
inode* inode::findFolderRecursive(std::vector<std::string> path, int pos, int cap) {
	inode* retINode = this;

	if (cap > pos) {
		std::string findFoldername = path.at(pos);

		// If it's a special case (..)
		if (findFoldername == "..") {
			retINode = (*parent).findFolderRecursive(path, ++pos, cap);
		}

		// If it's a special case (/)
		else if (findFoldername == "") {
			retINode = (*getRoot(*this)).findFolderRecursive(path, ++pos, cap);
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
int inode::findBlockId(std::string fileName) {
	int id = findFile(fileName);
	if (id != -1)
		return files[id];
	return -1;
}
