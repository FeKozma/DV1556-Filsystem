#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "memblockdevice.h"
#include "inode.h"

class FileSystem
{
private:
    MemBlockDevice *mMemblockDevice;
	inode inode;
	bool *availableBlocks;

	// Here you can add your own data structures
public:
    FileSystem(int blockSize = 250);
    ~FileSystem();

    /* These API functions need to be implemented
	   You are free to specify parameter lists and return values
    */

	// detta �r en testfunktion VARNING testfunktion
	int createFileOn(std::string testString, int blocknr = 1);
	std::string viewFileOn(int blocknr = 1);

    /* This function creates a file in the filesystem */
    // createFile(...)

    /* Creates a folder in the filesystem */
    // createFolderi(...);

    /* Removes a file in the filesystem */
    // removeFile(...);

    /* Removes a folder in the filesystem */
    // removeFolder(...);

    /* Function will move the current location to a specified location in the filesystem */
    // goToFolder(...);

    /* This function will get all the files and folders in the specified folder */
    // listDir(...);

    /* Add your own member-functions if needed */
};

#endif // FILESYSTEM_H