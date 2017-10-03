#include "inode.h"

class inode
{
public:
	const std::string NAMELESS_FOLDER = "no-name";

	std::vector<int> files; //vilket block �r upptgaget av filen
	std::vector<inode> folder;
	inode* parent;
	inode(inode* &parent) {
		this->parent = parent;
	}
	~inode() {};
	bool addFolder(std::string name = "")
	{
		bool folderAdded = false;

		//om mappnamn ej angivet. nemge mappen "namnl�s mapp"
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
	bool findFolder(std::string name)
	{
		bool retVal = false;
		for (std::vector<inode>::size_type i = 0; i != folder.size(); i++)
		{
			if (this->folder[i].getFolderName() == name)
			{
				retVal = true;
			}

		}
		return retVal;
	}
	std::string getFolderName()
	{
		return "undefined";
	}


};