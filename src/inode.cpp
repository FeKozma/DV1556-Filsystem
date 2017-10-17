#include "inode.h"

const std::string NAMELESS_FOLDER = "no-name";

// Used when initializing root.
inode::inode() {
	this->parent = this;
}

inode::inode(inode* &parent) {
	this->parent = parent;
}

//TODO: test ->have no access to UnitTest files "The project file or webb cannot be found."
bool inode::addFile(std::string name, int freeBlock, std::string path) {
	
	bool fileAdded = false;
	if (name != "")
	{
		std::vector<std::string> pathList = pathSplitter(path);

		inode* folder = findFolderRecursive(pathList, 0, pathList.capacity());
		//TODO: find path

		//TODO: ta första mapp namn -> kolla om du har denna map -> gå in i dennna mapp

		if (findFile(name) == -1) {
			// Add file
			filesName.push_back(name);
			files.push_back(freeBlock);

			fileAdded = true;
		}
	}
	return fileAdded;
}

int inode::findFile(std::string name)
{
	int filePos = -1;
	for (int i = 0; i <  filesName.size(); ++i) {
		if (filesName[i] == name)
		{
			filePos = i;
			i = filesName.size();
		}
	}
	return filePos;
}

bool inode::addFolder(std::string name, std::string path) {
	bool folderAdded = false;

	// If no name is entered, name the folder to what the variable NAMELESS_FOLDER is plus the variable i.
	if (name == "") {
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

inode* inode::goToFolder(std::string path) {
	std::vector<std::string> pathList = pathSplitter(path);
	return findFolderRecursive(pathList, 0, pathList.size());
}

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

std::string inode::getFolderName() {
	return this->name;
}

std::string inode::getFolderPath() {
	std::string path = "/";
	inode *current = this;
	while (current->getFolderName() != "") {
		path = "/" + current->getFolderName() + path;
		current = current->parent;
	}

	return path;
}

std::vector<std::string> inode::getFolders() {
	std::vector<std::string> folders;
	for (int i = 0; i < folder.size(); ++i) {
		folders.push_back(folder[i].getFolderName());
	}
	return folders;
}

std::vector<std::string> inode::getFiles() {
	return filesName;
}

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

//TODO: memory leak somewhere in this method.
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

inode* inode::getRoot(inode &current) {
	inode *retInode = &current;

	if (current.parent != &current) {
		retInode = getRoot(*current.parent);
	}

	return retInode;
}
