#include <string>
#include <iostream>
#include <fstream>
#include "numbergenerator.h"
#include "blockgenerator.h"
#include "block.h"
#include "board.h"
#include "sBlock.h"
#include "zBlock.h"
#include "iBlock.h"
#include "jBlock.h"
#include "lBlock.h"
#include "oBlock.h"
#include "tBlock.h"
#include "starBlock.h"


BlockGenerator::BlockGenerator(Board* board, NumberGenerator* ng, std::string sequenceFileName) 
    : board{board}, ng{ng}, sequenceFileName{sequenceFileName} {} // constructor


BlockGenerator::~BlockGenerator() {
    reset();
} // destructor 


// resets generator to its original state
void BlockGenerator::reset() {
    isRandom = true;
    noRandomFileName = "";
    
    if (sequenceReader.is_open()) {
        sequenceReader.close();
    } // if

    if (noRandomFileReader.is_open()) {
        noRandomFileReader.close();
    } // if
} // reset


// generates a block for level 0 (from sequence file)
std::string BlockGenerator::generateLevel0() {
    std::string type;

    if (!sequenceReader.is_open()) {
        sequenceReader.open(sequenceFileName.c_str());
    } // if

    sequenceReader >> type;

    if (sequenceReader.fail()) {
        sequenceReader.close();
        sequenceReader.open(sequenceFileName.c_str());
        sequenceReader >> type;
    } // if

    return type;
} // generateLevel0


// generate a block with level 1 probabilities
std::string BlockGenerator::generateLevel1(int random) {
    switch(random) {
        case 0:
            return "S";
        case 1:
            return "Z";
        case 2:
        case 3:
            return "I";
        case 4:
        case 5:
            return "J";
        case 6:
        case 7:
            return "L";
        case 8:
        case 9:
            return "O";
        case 10:
        case 11:
            return "T";
    } // switch

    return "";
} // generateLevel1


// generate a block with level 2 probabilities
std::string BlockGenerator::generateLevel2(int random) {
    switch(random) {
        case 0: 
            return "I";
        case 1:
            return "J";
        case 2:
            return "L";
        case 3:
            return "O";
        case 4:
            return "S";
        case 5:
            return "Z";
        case 6:
            return "T";
    } // switch    

    return "";
} // generateLevel2


// generate a block with level 3/4 probabilities
std::string BlockGenerator::generateLevel34(int random) {
    if (isRandom) {
        switch(random) {
            case 0:
            case 1:
                return "S";
            case 2:
            case 3:
                return "Z";
            case 4:
                return "I";
            case 5:
                return "J";
            case 6:
                return "L";
            case 7:
                return "O";
            case 8:
                return "T";
        } // switch
    } else {
        std::string type;

        if (!noRandomFileReader.is_open()) {
            noRandomFileReader.open(noRandomFileName.c_str());
        } // if

        noRandomFileReader >> type;

        if (noRandomFileReader.fail()) {
            noRandomFileReader.close();
            noRandomFileReader.open(noRandomFileName.c_str());
            noRandomFileReader >> type;
        } // if

        return type;
    } // if

    return "";
} // generateLevel34 


// generate a random number in [lower, upper]
int BlockGenerator::randNum(int lower, int upper) {
    return ng->randNum(lower, upper); 
} // randNum


// given a string representing the type of block, generate a new instance
// of the corresponding block
Block* BlockGenerator::makeBlock(std::string type, int level) {
    if (type == "S") {
        return new sBlock(level, board);
    } else if (type == "Z") {
        return new zBlock(level, board);
    } else if (type == "I") {
        return new iBlock(level, board);
    } else if (type == "J") {
        return new jBlock(level, board);
    } else if (type == "L") {
        return new lBlock(level, board);
    } else if (type == "O") {
        return new oBlock(level, board);
    } else if (type == "T") {
        return new tBlock(level, board);
    } else {
        return nullptr;
    } // if
} // makeBlock


// generates a block according to the given level (Player calls this)
Block* BlockGenerator::generateBlock(int level) {
    if (level == 0) {
        return makeBlock(generateLevel0(), level);
    } else if (level == 1) {
        return makeBlock(generateLevel1(randNum(0, 11)), level);
    } else if (level == 2) {
        return makeBlock(generateLevel2(randNum(0, 6)), level);
    } else {
        return makeBlock(generateLevel34(randNum(0, 8)), level);
    } // if
} // generateBlock


// set the ifstream for reading the file specified by a norandom command
void BlockGenerator::setStream(std::string fileName) {
    if (isRandom) {
        isRandom = false;
        noRandomFileName = fileName;
        noRandomFileReader.open(noRandomFileName.c_str());
    } // if
} // setStream


// unsets the ifstream for reading the file specified by norandom
// this is used when the random command is entered
void BlockGenerator::unsetStream() {
    if (!isRandom) {
        isRandom = true;
        noRandomFileName = "";
        noRandomFileReader.close();
    } // if
} // unsetStream();