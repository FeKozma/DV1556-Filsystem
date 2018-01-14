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

	int findFolder(const std::string &name = "") const;
	
	std::vector<std::string> pathSplitter(const std::string path) const;
	inode* findFolderRecursive(const std::vector<std::string> &path, const int pos, const int cap /*cap -> sluta vid cap ex kan finnas filnamn i slutet av path*/, const bool useWithKnowledge = false);
	inode* findFolderContainingFileRecursive(const std::string &path);
	inode* getRoot(inode& curent);
	//pos = 1 filename pos = 0 pathname
	std::string* getPathAndFileName(const std::string &path) const;
	bool renameFileGivenName(const std::string &oldFile, const std::string &newFile);
	
	/*get last dir or file in given path*/
	std::string getLast(const std::string &path) const;

	/*ignore last dir or file in given path*/
	std::string ignoreLast(const std::string &path) const;

public:
	
	inode();
	inode(inode* &parent);
	inode(std::ifstream &in, inode* parent = nullptr);
	
	~inode() {};
	
	std::string getFolderName() const;
	std::string getFolderPath() const;
	std::vector<std::string> getFolders() const;
	std::vector<std::string> getFiles() const;
	std::vector<int> getFilePos() const;
	int getMemPosGivenPosInArr(const int &pos) const;
	int getNrOfFolders() const;

	/*add file*/
	bool addFile(const std::string &name,const int freeBlock,const std::string &path);
	bool addFile(const std::string &path, const int freeBlock);

	/*remove file*/
	void removeFile(const std::string &name,const std::string &path = "");
	
	/*find file in this node*/
	int findFile(const std::string &name) const;
	
	/*add folder*/
	bool addFolder(std::string name = "");
	
	/*go to decired folder, if no input return this*/
	inode* goToFolder(const std::string &path);

	/*find a block given its path, including filename*/
	int findBlockIdPath(const std::string &pathName) ;
	
	/*find file in this node*/
	int findBlockId(const std::string &fileName) const;


	std::string listDir();

	/*rename file given old filename and new filename*/
	bool renameFileGivenPath(const std::string &oldFile, const std::string &newFile);

	/*update pos of file in this node*/
	bool updatePos(const std::string &filename, const int newPos);


};

#endif // INODE_H
