#ifndef MEMBLOCKDEVICE_H
#define MEMBLOCKDEVICE_H
#include "blockdevice.h"
#include <iostream>
#include <fstream>

class MemBlockDevice: public BlockDevice
{
private:
	bool *availableBlocks;
	int blocksCap;
	/* Writes a block */
	int writeBlock(int blockNr, const std::vector<char> &vec);
	
	// Makes it posible to save big files over multiple blocks.
	int writeBlock(int blockNr, const std::string &strBlock, const bool notLastBlock);
	
	// Adds the string as is to memmory. Needs to start correctly (0 or 1).
	int writeBlock(int blockNr, const std::string &strBlock);
	int writeBlock(int blockNr, const char cArr[]);     // Use with care, make sure that cArr has at least the same dimension as block

public:

	int findFree(int size = 1);

	MemBlockDevice(int nrOfBlocks = 250);
	MemBlockDevice(const MemBlockDevice &other);

	~MemBlockDevice();

	/* Operators */
	MemBlockDevice& operator=(const MemBlockDevice &other);
	Block &operator[] (int index) const;

	/* Returns amount of free blocks */
	int spaceLeft() const;

	/* Writes a block given no blocknr */
	int writeBlock(const std::vector<char> &vec);
	int writeBlock(const std::string &strBlock, int nrBlocks);
	int writeBlock(const std::string &strBlock);
	int writeBlock(const char cArr[]);     // Use with care, make sure that cArr has at least the same dimension as block
	
	/* Remove a block */
	bool rmBlock(int blockNr);
	/* Restore a block */
	bool addBlock(int blockNr);

	/* Reads a block */
	Block readBlock(int blockNr) const;

	/* Resets all the blocks */
	void reset();

	/* Return the size */
	int size() const;

	/* Return image of files */
	std::string filesImage();

	bool readFilesImage(std::ifstream &in);

	/* Formats the stystem. Returns nr of deleted files */
	int formatSys();

	/* Copy the block on the given pos to a new pos that is returned */
	int copyBlock(int pos);

	std::string getDiskAllLocations() const;

	int lengthOfFile(const int startPos) const;
};

#endif // MEMBLOCKDEVICE_H
