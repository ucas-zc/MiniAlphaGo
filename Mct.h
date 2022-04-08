#ifndef MCT_H
#define MCT_H

#include "ReturnCode.h"
#include "CalMachine.h"
#include <vector>

class Mct
{
public:
    // constructor
    Mct(int8_t player, int32_t depth,
        int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE],
        Mct *parent, NS_CALMACHINE::Coordinate *pre);

    // deconstructor
    ~Mct();

    // add children
    void addChild(NS_CALMACHINE::Coordinate move);

    // is full expand
    int32_t isFullExpand();

    // is terminal?
    int32_t isTerminal();

private:
    // remove a valid move
    void removeValidMove(NS_CALMACHINE::Coordinate move);

public:
    // player
    int8_t m_player;
    // depth
    int32_t m_depth;
    // number of expand
    int32_t m_N;
    // reward
    int32_t m_Q;
    // board
    int8_t m_board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE];
    // ptr of parent
    Mct *m_parent;
    NS_CALMACHINE::Coordinate *m_pre;
    // child list
    std::vector<Mct*> m_child;
    // list of valid moves
    std::vector<NS_CALMACHINE::Coordinate> m_remain_valid_moves;
};

#endif // MCT_H
