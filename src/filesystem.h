#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "inode.h"

class FileSystem
{
private:
	MemBlockDevice *mMemblockDevice;
	inode *curFolder;
	bool *availableBlocks;
	int nrOfBlocks;
	int fileSize;

public:
	FileSystem(int blockSize = 250, int fileSize = 512);
	~FileSystem();

	// These are test functions, WARNING, TODO
	int createFileOn(std::string testString, int blocknr = 1);
	std::string viewFileOn(int blocknr = 1);

	/* This function creates a file in the filesystem */
	bool createFile(std::string data ,std::string name, std::string path = "");

	/* Creates a folder in the filesystem */
	std::string createFolderi(std::string name = "", std::string path = "");

	/* Removes a file in the filesystem */
	// removeFile(...);

	/* Removes a folder in the filesystem */
	// removeFolder(...);

	/* Function will move the current location to a specified location in the filesystem */
	std::string goToFolder(std::string path);

	/* This function will return the current directly */
	std::string getCurrentPath();

	/* This function will get all the files and folders in the specified folder */
	std::string listDir(std::string path);
};

#endif // FILESYSTEM_H
