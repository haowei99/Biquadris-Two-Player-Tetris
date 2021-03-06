#include <string>
#include <iostream>
#include "player.h"
#include "blockgenerator.h"
#include "numbergenerator.h"
#include "board.h"
#include "block.h"
#include "window.h"

Player::Player(int x, int y, int startLevel, std::string sequenceFileName, NumberGenerator* ng, Xwindow* window) 
    : board{new Board(x, y, window)}, 
      startLevel{startLevel}, 
      sequenceFileName{sequenceFileName}, 
      generator{new BlockGenerator(board, ng, sequenceFileName)} {} // constructor 


Player::~Player() {
    delete generator;
    delete board;
    delete curBlock;
    delete nextBlock;
} // destructor


void Player::init(Player* opponent) {
    this->opponent = opponent;
    level = startLevel;
    score = 0;
    highscore = 0;
    curBlock = generator->generateBlock(level);
    nextBlock = generator->generateBlock(level);

    /*** ATTENTION!!!!! ***/
    board->init();
    board->set_blocks(curBlock, nextBlock);
    board->initBlock(curBlock->getBlockType());
} // init


void Player::reset() {
    delete curBlock;
    delete nextBlock;
    blocksDropped = 0;
    
    generator->reset();
    board->reset();
} // reset


void Player::left() {
    curBlock->left();
} // left


void Player::right() {
    curBlock->right();
} // right


void Player::down() {
    curBlock->down();
} // down


void Player::clockwise() {
    curBlock->rotateClockwise();
} // clockwise


void Player::counterclockwise() {
    curBlock->rotateCounterClockwise();
} // counterclockwise


void Player::levelUp() {
    if (level < maxLevel) {
        level += 1;
        curBlock->setCurLevel(level);
    } // if
} // levelUp


void Player::levelDown() {
    if (level > minLevel) {
        level -= 1;
        curBlock->setCurLevel(level);
    } // if
} // levelDown


void Player::random() {
    if (level > 2) {
        generator->unsetStream();
    } // if
} // random


void Player::noRandom(std::string noRandomFileName) {
    if (level > 2) {
        generator->setStream(noRandomFileName);
    } // if
} // noRandom


void Player::setBlock(std::string blockType) { // needs err checking for block?
    // check if I can replace current block with the specified blocktype
    // if so, do so and delete previous curBlock

    if (board->swapBlock(blockType[0])) {
        curBlock = board->getCurrBlock();
    } // if
} // setBlock


void Player::drop() {
    // add the dropped block to board's placedBlocks <-- this should be handled in board/block
    curBlock->drop();
    board->addBlock(curBlock);

    if (hasBlind) {
        hasBlind = false;
        board->unblind();
    } // if
    
    hasHeavy = false;
    hasForce = false;
    blocksDropped += 1;
} // drop


int Player::clearRows() {
    // clear filled rows, use returned val to calculated a score and add that to own score
    int rowsCleared = board->checkRows();

    if (rowsCleared > 0) {
        score += ((level + rowsCleared) * (level + rowsCleared));
    } // if 

    if (score > highscore) {
        highscore = score;
    } // if

    if (rowsCleared == 0) {
        if (blocksDropped % 5 == 0 && level >= 4) {
            board->dropStar(level);
            
            int clearedByStar = board->checkRows();

            if (clearedByStar > 0) {
                score += ((level + clearedByStar) * (level + clearedByStar));
                blocksDropped = 0;
            } // if 
        } // if
    } else {
        blocksDropped = 0;
    } // if

    return rowsCleared;
} // clearRows


void Player::checkRemovedBlocks() {
    // check for any removed blocks, add returned score to own score
    //    - call count_score()
    score += board->count_score();

    if (score > highscore) {
        highscore = score;
    } // if
} // checkRemoveBlocks


bool Player::endTurn() {
    // check if nextBlock can be placed on the top of the board
    //     if not: I lose, return true
    //     if so: curBlock = nextBlock
    //            nextBlock = generator->getNextBlock(), return false
    if (board->canFitNew(nextBlock->getBlockType())) {
        curBlock = nextBlock;
        nextBlock = generator->generateBlock(level);
        board->set_blocks(curBlock, nextBlock);
        board->initBlock(curBlock->getBlockType());

        return false;
    } // if

    curBlock = nullptr; // this is so player and board do not try deleting the same object
    return true;
} // endTurn


// applies special effects it their corresponding flags are set to true
// if player loses due to the force special action, retrur nfalse
// otherwise return true
bool Player::applyEffects() {
    curBlock->setCurLevel(level);
    
    if (hasHeavy) {
        curBlock->applyHeavy(); 
    } // if

    if (hasBlind) {
        board->blind();
    } // if

    if (hasForce) {
        if (!(board->swapBlock(forceType))) {
            return false;
        } else {
            curBlock = board->getCurrBlock();
        } // if
    } // if

    return true;
} // applyEffects


void Player::heavyOpponent() {
    opponent->hasHeavy = true;
} // heavyOpponent


void Player::forceOpponent(char type) {
    opponent->hasForce = true;
    opponent->forceType = type;
} // forceOpponent


void Player::blindOpponent() {
    opponent->hasBlind = true;
} // blindOpponent


int Player::getScore() {
    return score;
} // getScore


int Player::getHighscore() {
    return highscore;
} // getHighscore


int Player::getLevel() {
    return level;
} // getLevel


Board* Player::getBoard() {
    return board;
} // getBoard
