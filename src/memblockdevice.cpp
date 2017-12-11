#include "memblockdevice.h"
#include <stdexcept>

MemBlockDevice::MemBlockDevice(int nrOfBlocks): BlockDevice(nrOfBlocks) {
	this->availableBlocks = new bool[nrOfBlocks];
	this->blocksCap = nrOfBlocks;
	for (int i = 0; i < nrOfBlocks; i++)
	{
		availableBlocks[i] = true;
	}
}

MemBlockDevice::MemBlockDevice(const MemBlockDevice &other) : BlockDevice(other) {
	this->availableBlocks = new bool[nrOfBlocks];
	this->blocksCap = nrOfBlocks;
	for (int i = 0; i < nrOfBlocks; i++)
	{
		availableBlocks[i] = true;
	}
}

MemBlockDevice::~MemBlockDevice() {
	/* Implicit call to base-class destructor */
	delete[] 	availableBlocks;
}

MemBlockDevice& MemBlockDevice::operator=(const MemBlockDevice &other) {
	delete [] this->memBlocks;
	this->nrOfBlocks = other.nrOfBlocks;
	this->freePointer = other.freePointer;
	this->memBlocks = new Block[this->nrOfBlocks];

	for (int i = 0; i < this->nrOfBlocks; ++i)
		this->memBlocks[i] = other.memBlocks[i];

	return *this;
}

Block& MemBlockDevice::operator[](int index) const {
	if (index < 0 || index >= this->nrOfBlocks) {
		throw std::out_of_range("Illegal access\n");
	}
	else {
		return this->memBlocks[index];
	}
}

int MemBlockDevice::spaceLeft() const {
	int freeNr = 0;
	for (int i = 0; i < blocksCap; ++i)
	{
		if (availableBlocks[i] == true)
		{
			freeNr += 1;
		}
	}
	return freeNr;
}

int MemBlockDevice::writeBlock(const std::vector<char>& vec)
{
	int retVal = findFree(1);
	if (retVal != -1)
	{
		if (writeBlock(retVal, vec) != 1) {
			retVal = -1;
		}
		else
		{
			this->availableBlocks[retVal] = false;
		}
	}
	return retVal;
}

int MemBlockDevice::writeBlock(const std::string & strBlock)
{
	//Problem: -2 is returned!!

	int retVal = findFree(1);
	if (retVal != -1)
	{
		if (writeBlock(retVal, strBlock) != 1) {
			retVal = -1;
		}
		else
		{
			this->availableBlocks[retVal] = false;
		}
	}
	return retVal;
}

int MemBlockDevice::writeBlock(const char cArr[])
{
	int retVal = findFree(1);
	if (retVal != -1)
	{
		if (writeBlock(retVal, cArr) != 1) {
			retVal = -1;
		}
		else
		{
			this->availableBlocks[retVal] = false;
		}
	}
	return retVal;
}

void MemBlockDevice::rmBlock(int blockNr)
{
	if (blockNr >= 0)
	{
		availableBlocks[blockNr] = true;
	}
}

int MemBlockDevice::writeBlock(int blockNr, const std::vector<char> &vec) {
	int output = -1;    // Assume blockNr out-of-range

	if (blockNr < this->nrOfBlocks && blockNr >= 0) {
		/* -2 = vec and block dont have same dimensions */
		/* 1 = success */
		output = this->memBlocks[blockNr].writeBlock(vec);
	}
	return output;
}

int MemBlockDevice::writeBlock(int blockNr, const std::string &strBlock) {
	int output = -1;    // Assume blockNr out-of-range

	if (blockNr < this->nrOfBlocks && blockNr >= 0) {
		/* -2 = str-length and block dont have same dimensions */
		/* 1 = success */
		output = this->memBlocks[blockNr].writeBlock(strBlock);
	}
	return output;
}

int MemBlockDevice::writeBlock(int blockNr, const char cArr[]) {
	int output = -1;    // Assume blockNr out-of-range
	if (blockNr < this->nrOfBlocks && blockNr >= 0) {
		output = 1;
		// Underlying function writeBlock cannot check array-dimension.
		this->memBlocks[blockNr].writeBlock(cArr);
	}
	return output;
}

int MemBlockDevice::findFree(int size)
{
	int count = 0;
	int freeSpace = -1;
	for (int i = 0; i < blocksCap; ++i)
	{
		if (availableBlocks[i] == true)
		{
			count++;
			for (int k = 1; k < size; ++k)
			{
				if (availableBlocks[i + k] == true)
				{
					count++;
				}
			}
			if (count >= size)
			{
				freeSpace = i;
				i = blocksCap;
			}
			count = 0;
		}
	}
	return freeSpace;
}

Block MemBlockDevice::readBlock(int blockNr) const {
	if (blockNr < 0 || blockNr >= this->nrOfBlocks)
		throw std::out_of_range("Block out of range");
	else {
		Block a(this->memBlocks[blockNr]);
		return a;
	}
}

/* Resets all the blocks */
void MemBlockDevice::reset() {
	for (int i = 0; i < this->nrOfBlocks; ++i) {
		this->memBlocks[i].reset('0');
	}
}

int MemBlockDevice::size() const {
	return this->nrOfBlocks;
}

std::string MemBlockDevice::filesImage()
{
	std::string retString = "";
	for (int i = 0; i < this->blocksCap; ++i)
	{
		if (!availableBlocks[i])
		{
			retString += std::to_string(i) + ":";
			retString += readBlock(i).toString() + "\n";
		}
	}
	retString += "EOF\n";
	return retString;
}

bool MemBlockDevice::readFilesImage(std::ifstream& input)
{
	int pos;
	std::string save;
	bool success = false;
	//remove old data
	formatSys();
	
	if (input.is_open())
	{
		while(!input.eof())
		{
			input >> save;
			std::size_t found = save.find(":");
			if (found != std::string::npos)
			{
				pos = std::stoi(save.substr(0, found));
				this->availableBlocks[pos] = false;
				save.erase(0, pos+2);
				writeBlock(pos, save);
			}
 		}
		success = true;
	}
	return success;
}

int MemBlockDevice::formatSys()
{
	int retVal = 0;
	for (int i = 0; i < this->blocksCap; ++i)
	{
		if (this->availableBlocks[i] == false)
		{
			retVal++;
			this->availableBlocks[i] = true;
		}
	}
	return retVal;
}
