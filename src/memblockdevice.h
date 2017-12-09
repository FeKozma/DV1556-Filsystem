#ifndef MEMBLOCKDEVICE_H
#define MEMBLOCKDEVICE_H
#include "blockdevice.h"

class MemBlockDevice: public BlockDevice
{
private:
	bool *availableBlocks;
	int blocksCap;
	/* Writes a block */
	int writeBlock(int blockNr, const std::vector<char> &vec);
	int writeBlock(int blockNr, const std::string &strBlock);
	int writeBlock(int blockNr, const char cArr[]);     // Use with care, make sure that cArr has at least the same dimension as block

	int findFree(int size = 1);
public:
	MemBlockDevice(int nrOfBlocks = 250);
	MemBlockDevice(const MemBlockDevice &other);

	~MemBlockDevice();

	/* Operators */
	MemBlockDevice& operator=(const MemBlockDevice &other);
	Block &operator[] (int index) const;

	/* Returns amount of free blocks */
	int spaceLeft() const;

	/* Writes a block given no blocknr*/
	int writeBlock(const std::vector<char> &vec);
	int writeBlock(const std::string &strBlock);
	int writeBlock(const char cArr[]);     // Use with care, make sure that cArr has at least the same dimension as block
	
	/*remove a block*/
	void rmBlock(int blockNe);

	/* Reads a block */
	Block readBlock(int blockNr) const;

	/* Resets all the blocks */
	void reset();

	/* Return the size */
	int size() const;
};

#endif // MEMBLOCKDEVICE_H
