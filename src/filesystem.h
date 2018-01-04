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
	//TODO:? fileSize borde kanske flyttas till mmemblockdevice
	int fileSize;

	//returnerar sträng som ska sparas
	void createImageRecusive(inode *root, std::string & uoutput);
public:
	FileSystem(const int & blockSize = 250,const int & fileSize = 512);
	~FileSystem();

	bool createImage(std::string filename);

	bool loadImage(std::string filename = "data");


	// These are test functions, WARNING, TODO
	int createFileOn(std::string testString);

	/* This function will list the content of a file */
	std::string viewFileOn(const std::string & fileName);
	std::string viewFileOn(int pos) const;

	std::string stringTrim(std::string &s) const;

	/* This function creates a file in the filesystem */
	bool createFile(std::string content , std::string path = "");

	/* Creates a folder in the filesystem */
	std::string createFolderi(std::string path = "");

	/* Removes a file in the filesystem */
	bool removeFile(std::string fileName);

	/* Removes a folder in the filesystem */
	// removeFolder(...);

	/* Function will move the current location to a specified location in the filesystem */
	std::string goToFolder(std::string path);

	/* This function will return the current directly */
	std::string getCurrentPath();

	/* This function will get all the files and folders in the specified folder */
	std::string listDir(std::string path);
	std::string listDir();


	/* This function will format the system. return nr of blocks removed */
	int formatSystem();

	/**/
	bool copyFile(std::string oldFile, std::string newFile);

	bool renameFileGivenPath(std::string oldFile, std::string newFile);

	std::string ignoreLast(const std::string &path) const;
	std::string getLast(const std::string &path) const;

	std::string getDiskAllocations();

	bool appendFile(std::string file1, std::string file2);
};

#endif // FILESYSTEM_H
