#include "memblockdevice.h"
#include <stdexcept>

MemBlockDevice::MemBlockDevice(int nrOfBlocks): BlockDevice(nrOfBlocks) {
	this->availableBlocks = new bool[nrOfBlocks];
	this->permissionBlocks = new int[nrOfBlocks];
	this->blocksCap = nrOfBlocks;
	for (int i = 0; i < nrOfBlocks; i++) {
		availableBlocks[i] = true;
	}
	for (int i = 0; i < nrOfBlocks; ++i) {
		permissionBlocks[i] = 6;
	}
}

MemBlockDevice::MemBlockDevice(const MemBlockDevice &other) : BlockDevice(other) {
	this->availableBlocks = new bool[nrOfBlocks];
	this->permissionBlocks = new int[nrOfBlocks];
	this->blocksCap = nrOfBlocks;
	for (int i = 0; i < nrOfBlocks; i++) {
		availableBlocks[i] = true;
	}
	for (int i = 0; i < nrOfBlocks; ++i) {
		permissionBlocks[i] = 6;
	}
}

MemBlockDevice::~MemBlockDevice() {
	/* Implicit call to base-class destructor */
	delete[] availableBlocks;
	delete[] permissionBlocks;
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
	for (int i = 0; i < blocksCap; ++i) {
		if (availableBlocks[i] == true) {
			freeNr += 1;
		}
	}
	return freeNr;
}

int MemBlockDevice::writeBlock(const std::vector<char>& vec) {
	int retVal = findFree(1);
	if (retVal != -1) {
		if (writeBlock(retVal, vec) != 1) {
			retVal = -1;
		}
		else {
			this->availableBlocks[retVal] = false;
		}
	}
	return retVal;
}

int MemBlockDevice::writeBlock(const std::string & strBlock, int nrBlocks) {
	int retVal = findFree(nrBlocks);
	if (retVal != -1) {
		for (int i = 0; i < nrBlocks; ++i) {
			if (writeBlock(retVal+i, strBlock.substr((i)*511, 511), nrBlocks -i -1) != 1) {
				retVal = -1;
				i = nrBlocks;
			}
			else {
				this->availableBlocks[retVal+i] = false;
			}
		}
	}
	return retVal;
}

int MemBlockDevice::writeBlock(const std::string & strBlock) {
	int retVal = findFree(1);
	if (retVal != -1) {
		if (writeBlock(retVal, strBlock) != 1) {
			retVal = -1;
		}
		else {
			this->availableBlocks[retVal] = false;
		}
	}
	return retVal;
}

int MemBlockDevice::writeBlock(const char cArr[]) {
	int retVal = findFree(1);
	if (retVal != -1) {
		if (writeBlock(retVal, cArr) != 1) {
			retVal = -1;
		}
		else {
			this->availableBlocks[retVal] = false;
		}
	}
	return retVal;
}

bool MemBlockDevice::rmBlock(int blockNr) {
	bool retVal = false;
	if (blockNr >= 0) {
		if (this->memBlocks[blockNr].getIfMore()) {
			if (rmBlock(blockNr + 1)) {
				availableBlocks[blockNr] = true;
				permissionBlocks[blockNr] = 6;
				retVal = true;
			}
		}
		else {
			availableBlocks[blockNr] = true;
			permissionBlocks[blockNr] = 6;
			retVal = true;
		}
	}
	return retVal;
}

bool MemBlockDevice::addBlock(int blockNr) {
	bool retVal = false;
	if (blockNr >= 0) {
		if (this->memBlocks[blockNr].getIfMore()) {
			if (addBlock(blockNr + 1)) {
				availableBlocks[blockNr] = false;
				retVal = true;
			}
		}
		else {
			availableBlocks[blockNr] = false;
			retVal = true;
		}
	}
	return retVal;
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

int MemBlockDevice::writeBlock(int blockNr, const std::string &strBlock, const bool notLastBlock) {
	int output = -1;    // Assume blockNr out-of-range

	if (blockNr < this->nrOfBlocks && blockNr >= 0) {
		/* -2 = str-length and block dont have same dimensions */
		/* 1 = success */
		if (notLastBlock) {
			output = this->memBlocks[blockNr].writeBlock('0' + strBlock);
		}
		else {
			output = this->memBlocks[blockNr].writeBlock('1' + strBlock);
		}
	}
	return output;
}

int MemBlockDevice::writeBlock(int blockNr, const std::string & strBlock)
{
	int output = -1; // Assume blockNr out-of-range

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

int MemBlockDevice::findFree(int size) {
	int count = 0;
	int freeSpace = -1;
	for (int i = 0; i < blocksCap; ++i) {
		if (availableBlocks[i] == true) {
			count++;
			for (int k = 1; k < size; ++k) {
				if (availableBlocks[i + k] == true) {
					count++;
				}
			}
			if (count >= size) {
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

std::string MemBlockDevice::filesImage() {
	std::string retString = "";
	for (int i = 0; i < this->blocksCap; ++i) {
		if (!availableBlocks[i]) {
			retString += std::to_string(i) + ":";
			retString += readBlock(i).toString() + "\n";
		}
	}
	retString += "EOF\n";
	return retString;
}

bool MemBlockDevice::readFilesImage(std::ifstream &input) {
	int pos;
	std::string save;
	bool success = false;

	// Remove old data
	formatSys();
	
	if (input.is_open()) {
		while(!input.eof()) {
			input >> save;
			std::size_t found = save.find(":");
			if (found != std::string::npos) {
				pos = std::stoi(save.substr(0, found));
				this->availableBlocks[pos] = false;
				save.erase(0, found+1);
				writeBlock(pos, save);
			}
 		}
		success = true;
	}
	return success;
}

int MemBlockDevice::formatSys() {
	int retVal = 0;
	for (int i = 0; i < this->blocksCap; ++i) {
		if (this->availableBlocks[i] == false) {
			retVal++;
			this->availableBlocks[i] = true;
		}
	}
	return retVal;
}

int MemBlockDevice::copyBlock(int pos) {
	int newPos = -1;
	int length = this->lengthOfFile(pos);
	if (length > 0) {
		newPos = this->findFree(length);
		for (int i = 0; i < length; ++i) {
			writeBlock(newPos + i, this->readBlock(pos + i).toString());
			this->availableBlocks[newPos + i] = false;
		}
		//writeBlock(newPos, this->readBlock(pos).toString());
	}
	return newPos;
}

std::string MemBlockDevice::getDiskAllocations() const {
	std::string retString = "";
	for (int i = 0; i < this->blocksCap - 4; i = i + 4) {
		retString += std::to_string(i) + ": ";
		if (availableBlocks[i])
			retString += "empty	";
		else
			retString += "full	";
		
		retString += std::to_string(i+1) + ": ";
		if (availableBlocks[i+1])
			retString += "empty	";
		else
			retString += "full	";

		retString += std::to_string(i+2) + ": ";
		if (availableBlocks[i+2])
			retString += "empty	";
		else
			retString += "full	";

		retString += std::to_string(i+3) + ": ";
		if (availableBlocks[i+3])
			retString += "empty	";
		else
			retString += "full	";

		retString += "\n";

	}
	return retString;
}

int MemBlockDevice::lengthOfFile(const int startPos) const {
	int spaces = 0;
	if (startPos < this->blocksCap) {
		// std::cout << this->memBlocks[startPos].getIfMore() ? "true\n" : "False\n";
		while ((startPos + spaces) < this->blocksCap && this->memBlocks[startPos + spaces].getIfMore()) {
			spaces++;
		}
		spaces++;
	}
	return spaces;
}

std::string MemBlockDevice::getPermissionType(const int blockId) const {
	std::string type = "";
	if (permissionBlocks[blockId] == 2) {
		type = "w";
	}
	else if (permissionBlocks[blockId] == 4) {
		type = "r";
	}
	else if (permissionBlocks[blockId] == 6) {
		type = "rw";
	}
	return type;
}

bool MemBlockDevice::changePermissionType(const int blockId, const int newType) {
	bool permissionChanged = false;
	if (newType == 2 || newType == 4 || newType == 6) {
		permissionBlocks[blockId] = newType;
		permissionChanged = true;
	}
	return permissionChanged;
}

bool MemBlockDevice::hasPermissionWrite(const int blockId) const {
	return permissionBlocks[blockId] == 2 || hasPermissionBoth(blockId);
}

bool MemBlockDevice::hasPermissionRead(const int blockId) const {
	return permissionBlocks[blockId] == 4 || hasPermissionBoth(blockId);
}

bool MemBlockDevice::hasPermissionBoth(const int blockId) const {
	return permissionBlocks[blockId] == 6;
}
