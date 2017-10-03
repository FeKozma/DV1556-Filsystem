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
public:
	
	inode(inode* &parent);
	
	~inode() {};

	bool addFolder(std::string name = "");
	
};

#endif
