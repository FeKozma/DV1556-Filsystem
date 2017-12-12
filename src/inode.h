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
	int getNrOfFolders() const;

	bool addFile(std::string name, int freeBlock, std::string path = "");
	void removeFile(std::string name, std::string path = "");
	int findFile(std::string name) const;
	bool addFolder(std::string name = "", std::string path = "");
	inode* goToFolder(std::string path);

	int findBlockIdPath(std::string pathName) ;
	int findBlockId(std::string fileName);
	std::string getLast(std::string path);

	std::string listDir();
};

#endif // INODE_H
