#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>

class inode {
private:
	//Where this folder exists
	inode* parent;
	//This folder name
	std::string name; 
	//Which block is busy by the file
	std::vector<int> files; 
	// filesName överstämmer med files. filesName.at(1) sparas på position som sparas i files.at(1)
	std::vector<std::string> filesName;  
	//Links to folders in this folder
	std::vector<inode> folder; 

	int findFolder(std::string name = "");
	
	std::vector<std::string> pathSplitter(std::string path);
	inode* findFolderRecursive(std::vector<std::string> path, int pos, int cap /*cap -> sluta vid cap ex kan finnas filnamn i slutet av path*/);
	inode* getRoot(inode& curent);

	
public:
	
	inode();
	inode(inode* &parent);
	
	~inode() {};
	
	std::string getFolderName();
	std::string getFolderPath();
	std::vector<std::string> getFolders();
	std::vector<std::string> getFiles();

	bool addFile(std::string name, int freeBlock, std::string path = "");
	void removeFile(std::string name, std::string path = "");
	int findFile(std::string name);
	bool addFolder(std::string name = "", std::string path = "");
	inode* goToFolder(std::string path);

	int findBlockId(std::string fileName);
};

#endif // INODE_H
