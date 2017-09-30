#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"



class FileSystem
{
public:

private:
    MemBlockDevice mMemblockDevice;

	class inode
	{
	public:
		const std::string NAMELESS_FOLDER = "no-name";

		std::vector<int> files; //vilket block är upptgaget av filen
		std::vector<inode> folder;
		inode* parent;
		inode(inode* &parent) {
			this->parent = parent;
		}
		~inode() {};
		bool addFolder(std::string name = "")
		{
			//om mappnamn ej angivet. nemge mappen "namnlös mapp"
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

			}

			if (!findFolder(name))
			{
				//create folder
			}
			

			return false;
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


    // Here you can add your own data structures
public:
    FileSystem();
    ~FileSystem();

	

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

	// detta är en testfunktion VARNING testfunktion
	int createFileOn(int blocknr = 1);
	std::string viewFileOn(int blocknr = 1);

    /* This function creates a file in the filesystem */
    // createFile(...)

    /* Creates a folder in the filesystem */
    // createFolderi(...);

    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Removes a folder in the filesystem */
    // removeFolder(...);

    /* Function will move the current location to a specified location in the filesystem */
    // goToFolder(...);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);

    /* Add your own member-functions if needed */
};

#endif // FILESYSTEM_H
