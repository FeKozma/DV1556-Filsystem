#ifndef INODE_H
#define INODE_H

#include <string>
#include <vector>

class inode {
private:
	inode* parent;
	std::string name;
	std::vector<int> files; //vilket block är upptgaget av filen
	std::vector<std::string> filesName;  //filesName överstämmer med files. fileName 1 sparas på files.at(1)
	std::vector<inode> folder;

	int findFolder(std::string name = "");
	std::string getFolderName();
	std::vector<std::string> pathSpliter(std::string path);
	inode* findFolderRecusive(std::vector<std::string> path, int pos, int cap /*cap -> sluta vid cap ex kan finnas filnamn i slutet av path*/);
	inode* getRoot(inode& curent);
public:
	
	inode(inode* &parent, std::string folderName);
	
	~inode() {};
	int addFile(std::string name, int freeBlock, std::string path = "");
	bool addFolder(std::string name = "" , std::string path = "");
	inode* goToFolder(std::string path);
};

#endif
