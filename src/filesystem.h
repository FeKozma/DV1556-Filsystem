#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "inode.h"
#include <algorithm>
#include <cctype>

class FileSystem {
private:
	MemBlockDevice *mMemblockDevice;
	inode *curFolder;
	bool *availableBlocks;
	int nrOfBlocks;
	int fileSize;

	//returnerar sträng som ska sparas
	void createImageRecusive(inode *root, std::string & uoutput);
public:
	FileSystem(int blockSize = 250, int fileSize = 512);
	~FileSystem();

	bool createImage(std::string filename = "data", std::string path = "");

	// These are test functions, WARNING, TODO
	int createFileOn(std::string testString, int blocknr = 1);

	/* This function will list the content of a file */
	std::string viewFileOn(std::string fileName);

	void stringTrim(std::string &s);

	/* This function creates a file in the filesystem */
	bool createFile(std::string content ,std::string name, std::string path = "");

	/* Creates a folder in the filesystem */
	std::string createFolderi(std::string name = "", std::string path = "");

	/* Removes a file in the filesystem */
	void removeFile(std::string fileName);

	/* Removes a folder in the filesystem */
	// removeFolder(...);

	/* Function will move the current location to a specified location in the filesystem */
	std::string goToFolder(std::string path);

	/* This function will return the current directly */
	std::string getCurrentPath();

	/* This function will get all the files and folders in the specified folder */
	std::string listDir(std::string path);

	/* This function will format the system. return nr of blocks removed */
	int formatSystem();
};

#endif // FILESYSTEM_H
