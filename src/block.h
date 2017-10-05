#ifndef BLOCK_H
#define BLOCK_H

#include <string>
#include <vector>
#include <iostream>

class Block
{
private:
    char *block;
    int nrOfElements;

public:
    /* Constructor */
    Block(int nrOfElements = 512);    // overloaded (default) constructor
    Block(const Block &other); // copy-constructor

    /* Destructor */
    ~Block();

    /* Operators */
    Block& operator = (const Block &other);  // Assignment operator
    char operator[] (int index) const;  // []-operator
    friend std::ostream& operator<<(std::ostream &os, const Block& blck)
    {
        for (int i = 0; i < blck.nrOfElements; ++i)
            os << blck.block[i];
        return os;
    }

    void reset(char c = 0);  // Sets every element in char-array to 0
    int size() const;   // returns the size
    Block readBlock() const;    // Returns a copy of block

    /* Write a block */
    int writeBlock(const std::string &strBlock);
    int writeBlock(const std::vector<char> &vec);
    void writeBlock(const char cArr[]);     // Use with caution! Make sure that cArr is at least as large as private member block.

    std::string toString() const;
};

#endif // BLOCK_H
