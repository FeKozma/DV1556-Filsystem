#include "inode.h"

const std::string NAMELESS_FOLDER = "no-name";

inode::inode(std::string folderName) {
	this->parent = this;
	this->name = folderName;
}

inode::inode(std::string folderName, inode* &parent) {
	this->parent = parent;
	this->name = folderName;
}

int inode::addFile(std::string name, int freeBlock, std::string path)
{
	//TODO: find path
	
	std::vector<std::string> pathList = pathSplitter(path);
	//TODO: ta första mapp namn -> kolla om du har denna map -> gå in i dennna mapp

	return 0;
}

bool inode::addFolder(std::string name, std::string path)
{
	bool folderAdded = false;

	// Om mappnamn ej angivet. namge mappen NAMELESS_FOLDER
	if (name == "") {
		int i = 0;
		name = NAMELESS_FOLDER;
		int sizeName = name.size();
		while (findFolder(name) >= 0)
		{
			i++;
			name.resize(7);
			name += "(" + std::to_string(i) + ")";
		}
		folderAdded = true;
	}
	else if (findFolder(name) == -1) {
		folderAdded = true;
	}

	// Add folder
	if (folderAdded) {
		folder.push_back(inode(name, goToFolder(path)->parent));
	}

	return folderAdded;
}

inode* inode::goToFolder(std::string path)
{
	std::vector<std::string> pathList = pathSplitter(path);
	return findFolderRecusive(pathList, 0, pathList.size());
}

int inode::findFolder(std::string name)
{
	int folderPos = -1;
	for (std::vector<inode>::size_type i = 0; i != folder.size(); i++)
	{
		if (this->folder[i].getFolderName() == name)
		{
			folderPos = i;
			i == folder.size();
		}
	}
	return folderPos;
}

std::string inode::getFolderName()
{
	return this->name;
}

std::vector<std::string> inode::pathSplitter(std::string path)
{
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

inode* inode::findFolderRecusive(std::vector<std::string> path, int pos, int cap)
{ //TODO: test funktion och möjligtvis reducera antalet "return"
	if (cap > pos)   //size -> correkt?   path.length()?
	{
		std::string findFoldername = path.at(pos);
		inode* next;
		// Om det är ett specialfall ex ..
		if (findFoldername == "..") {
			next = (*parent).findFolderRecusive(path, ++pos, cap);
			//TODO: osäker på om åvanstående rad kod fungerar korrekt
		}
		// Om det är ett specialfall /
		else if (findFoldername == "") {
			next = (*getRoot(*this)).findFolderRecusive(path, ++pos, cap); //TODO: work?
		}

		// Om findFolder är ett mappnamn
		int folderPos = findFolder(findFoldername);
		if (folderPos != -1) {
			next = folder.at(folderPos).findFolderRecusive(path, ++pos, cap);
		}
		else {
			return this;
		}

		return next;
	}
	else
	{
		return this;
	}
}

inode* inode::getRoot(inode &current)
{
	inode *retInode = &current;

	if (current.parent != &current) {
		retInode = getRoot(*current.parent);
	}

	return retInode;
}

