#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>

class inode {
private:
	inode* parent;
	std::vector<int> files; //vilket block är upptgaget av filen
	std::vector<inode> folder;

	bool findFolder(std::string name = "");
	std::string getFolderName();
	std::vector<std::string> pathSpliter(std::string path);
public:
	
	inode(inode* &parent, std::string folderName);
	
	~inode() {};
	int addFile(std::string name, int freeBlock, std::string path = "");
	bool addFolder(std::string name = "" , std::string path = "");
	
};

#endif
