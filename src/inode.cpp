#include "inode.h"

const std::string NAMELESS_FOLDER = "no-name";



inode::inode(inode* &parent, std::string folderName) {
	this->parent = parent;
}

int inode::addFile(std::string name, int freeBlock, std::string path)
{
	//TODO: find path
	
	std::vector<std::string> pathList = pathSpliter(path);
	//TODO: ta första mapp namn -> kolla om du har denna map -> gå in i dennna mapp

	return 0;
}

bool inode::addFolder(std::string name, std::string path)
{
	bool folderAdded = false;

	//om mappnamn ej angivet. namge mappen NAMELESS_FOLDER
	if (name == "")
	{
		int i = 0;
		name = NAMELESS_FOLDER;
		int sizeName = name.size();
		while (findFolder(name))
		{
			i++;
			name.resize(7);
			name += "(" + std::to_string(i) + ")";
		}
		//create folder
		folderAdded = true;
	}
	else if (!findFolder(name))
	{
		//TODO: create folder
		folderAdded = true;
	}


	return folderAdded;
}

inode * inode::goToFolder(std::string path)
{
	//TODO: splita path
	//TODO: calla på findFolderRecusive
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

	for (int i = 0; i < path.length(); i++)
	{
		if (path[i] == '/')
		{
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
{ //TODO: test function
	if (cap > pos)   //size -> correkt?   path.length()?
	{
		std::string findFoldername = path.at(pos);
		int folderPos = findFolder(findFoldername);
		if (folderPos != -1)
		{
			return folder.at(folderPos).findFolderRecusive(path, ++pos, cap);
		}
		else
		{
			return nullptr;
		}
	}
	else
	{
		return this;
	}
}
