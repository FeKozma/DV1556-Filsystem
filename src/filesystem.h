#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "inode.h"
#include <algorithm>
#include <cctype>
#include <iostream>
#include <fstream>

class FileSystem {
private:
	MemBlockDevice *mMemblockDevice;
	inode *curFolder;
	int fileSize;

	// Returns a string that is going to be saved.
	void createImageRecursive(inode *root, std::string &output);

	int createFileOn(std::string testString);

public:
	FileSystem(const int &blockSize = 250,const int &fileSize = 512);
	~FileSystem();

	bool createImage(std::string filename);

	bool loadImage(std::string filename = "data");

	// This function will list the content of a file.
	std::string viewFileOn(const std::string &fileName) const;
	std::string viewFileOn(int pos) const;

	std::string stringTrim(std::string &s) const;

	// This function creates a file in the filesystem.
	bool createFile(const std::string &content , const std::string &path = "");

	// Creates a folder in the filesystem.
	std::string createFolderi(std::string path = "");

	// Removes a file in the filesystem.
	bool removeFile(const std::string &fileName);

	// Removes a folder in the filesystem.
	// removeFolder(...);

	// Function will move the current location to a specified location in the filesystem.
	std::string goToFolder(const std::string &path);

	// This function will return the current directly.
	std::string getCurrentPath() const;

	// This function will get all the files and folders in the specified folder.
	std::string listDir(std::string path);
	std::string listDir();

	// This function will format the system. Return the nr of blocks removed.
	int formatSystem();

	// Will copy a file from one location to another.
	bool copyFile(const std::string &oldFile, std::string newFile);

	bool renameFileGivenPath(const std::string &oldFile, const std::string &newFile);

	// Will ignore what's after the last forward slash.
	std::string ignoreLastSlash(const std::string &path) const;

	// Will return what's after the last forward slash.
	std::string getAfterLastSlash(const std::string &path) const;

	std::string getDiskAllocations() const;

	bool appendFile(const std::string &file1, const std::string &file2);
};

#endif // FILESYSTEM_H
