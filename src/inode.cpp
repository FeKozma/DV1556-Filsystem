#include "inode.h"

const std::string NAMELESS_FOLDER = "no-name";

inode::inode(inode* &parent, std::string folderName) {
	this->parent = parent;
}

int inode::addFile(std::string name, int freeBlock, std::string path)
{
	//TODO: find path
	
	std::vector<std::string> pathList = pathSpliter(path);
	//TODO: ta f�rsta mapp namn -> kolla om du har denna map -> g� in i dennna mapp

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
		while (findFolder(name))
		{
			i++;
			name.resize(7);
			name += "(" + std::to_string(i) + ")";
		}
		//TODO: create folder
		folderAdded = true;
	}
	else if (!findFolder(name)) {
		//TODO: create folder
		folderAdded = true;
	}

	return folderAdded;
}

inode * inode::goToFolder(std::string path)
{
	//TODO: splita path
	//TODO: calla p� findFolderRecusive
	//TODO: return findFolderRecusive resultat
	return nullptr;
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

std::vector<std::string> inode::pathSpliter(std::string path)
{
	std::vector<std::string> retPath;

	int start = 0;

	for (int i = 0; i < path.length(); ++i) {
		if (path[i] == '/') {
			retPath.push_back(path.substr(start, i - start));
			start = i + 1;
		}
	}

	if (start != path.length()) {
		retPath.push_back(path.substr(start));
	}

	return retPath;
}

inode * inode::findFolderRecusive(std::vector<std::string> path, int pos, int cap)
{ //TODO: test funktion och m�jligtvis reducera antalet "return"
	if (cap > pos)   //size -> correkt?   path.length()?
	{
		std::string findFoldername = path.at(pos);
		inode* next;
		// Om det �r ett specialfall ex ../ eller /
		if (findFoldername == "..") {
			next = (*parent).findFolderRecusive(path, ++pos, cap);
			//TODO: os�ker p� om �vanst�ende rad kod fungerar korrekt
		}
		// Om det �r /
		else if (findFoldername == "") {
			next = (*getRoot(*this)).findFolderRecusive(path, ++pos, cap);   //TODO: work?
		}

		// Om findFolder �r ett mappnamn
		int folderPos = findFolder(findFoldername);
		if (folderPos != -1) {
			next = folder.at(folderPos).findFolderRecusive(path, ++pos, cap);
		}
		else {
			return nullptr;
		}

		return next;
	}
	else
	{
		return this;
	}
}

inode * inode::getRoot(inode& current)
{
	if (current.parent != &current) {
		return getRoot(*current.parent);
	}
	return &current;
}

