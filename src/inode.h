#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>
#include <iostream>
#include <fstream>

class inode {
private:
	inode* parent; // Where this folder exists
	std::string name; // This folder name
	std::vector<int> files; // Which block is busy by the file
	std::vector<std::string> filesName; // 'filesName' correspond to 'files'. filesName.at(1) will be saved at the position files.at(1).
	std::vector<inode> folder; // Links to folders in this folder

	int findFolder(std::string name = "") const;
	
	std::vector<std::string> pathSplitter(const std::string path) const;
	inode* findFolderRecursive(std::vector<std::string> path, int pos, int cap /*cap -> sluta vid cap ex kan finnas filnamn i slutet av path*/, bool useWithKnowledge = false) ;
	inode* findFolderContainingFileRecursive( std::string path) ;
	inode* getRoot(inode& curent);
	//pos = 1 filename pos = 0 pathname
	std::string* getPathAndFileName(std::string path);
	bool renameFileGivenName(std::string oldFile, std::string newFile);
	
	/*get last dir or file in given path*/
	std::string getLast(std::string path);

	/*ignore last dir or file in given path*/
	std::string ignoreLast(const std::string &path) const;

public:
	
	inode();
	inode(inode* &parent);
	inode(std::ifstream& in, inode* parent = nullptr);
	
	~inode() {};
	
	std::string getFolderName() const;
	std::string getFolderPath() const;
	std::vector<std::string> getFolders() const;
	std::vector<std::string> getFiles() const;
	std::vector<int> getFilePos() const;
	int getMemPosGivenPosInArr(const int &pos) const;
	int getNrOfFolders() const;

	/*add file*/
	bool addFile(std::string name, int freeBlock, std::string path);
	bool addFile(std::string path, int freeBlock);

	/*remove file*/
	void removeFile(std::string name, std::string path = "");
	
	/*find file in this node*/
	int findFile(std::string name) const;
	
	/*add folder*/
	bool addFolder(std::string name = "", std::string path = "");
	
	/*go to decired folder, if no input return this*/
	inode* goToFolder(std::string path);

	/*find a block given its path, including filename*/
	int findBlockIdPath(std::string pathName) ;
	
	/*find file in this node*/
	int findBlockId(std::string fileName);


	std::string listDir();

	/*rename file given old filename and new filename*/
	bool renameFileGivenPath(std::string oldFile, std::string newFile);

	/*update pos of file in this node*/
	bool updatePos(const std::string &filename, const int newPos);


};

#endif // INODE_H
