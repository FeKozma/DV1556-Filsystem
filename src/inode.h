#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>

class inode {
private:
	inode* parent;
	std::string name;
	std::vector<int> files; // Which block is busy by the file
	std::vector<std::string> filesName;  // filesName överstämmer med files. filesName 1 sparas på files.at(1)
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
	int findFile(std::string name);
	bool addFolder(std::string name = "", std::string path = "");
	inode* goToFolder(std::string path);
};

#endif // INODE_H
