#ifndef CALMACHINE_H
#define CALMACHINE_H

#include "ReturnCode.h"
#include <vector>

namespace NS_CALMACHINE {
    // board size
    static const int32_t BOARD_SIZE = 8;
    // priority table size
    static const int32_t PRIORITY_SIZE = 10;
    // null status
    static const int8_t NULL_CHESS = -1;
    // black chess status
    static const int8_t BLACK_CHESS = 0;
    // white chess status
    static const int8_t WHITE_CHESS = 1;

    // Coordinate
    typedef struct Coordinate
    {
        int32_t CoorX;
        int32_t CoorY;
    }Coordinate;

    typedef struct CoordinateArr{
        const NS_CALMACHINE::Coordinate Coor[NS_CALMACHINE::BOARD_SIZE];
    }CoordinateArr;

    //priority table
    static const NS_CALMACHINE::CoordinateArr priority_table[NS_CALMACHINE::PRIORITY_SIZE] = {
        {(0, 0), (0, 7), (7, 0), (7, 7)},
        {(0, 2), (0, 5), (2, 0), (5, 0), (2, 7), (5, 7), (7, 2), (7, 5)},
        {(2, 2), (2, 5), (5, 2), (5, 5)},
        {(3, 0), (4, 0), (0, 3), (0, 4), (7, 3), (7, 4), (3, 7), (4, 7)},
        {(3, 2), (4, 2), (2, 3), (2, 4), (3, 5), (4, 5), (5, 3), (5, 4)},
        {(3, 3), (4, 4), (3, 4), (4, 3)},
        {(1, 3), (1, 4), (3, 1), (4, 1), (6, 3), (6, 4), (3, 6), (4, 6)},
        {(1, 2), (1, 5), (2, 1), (5, 1), (6, 2), (6 ,5), (2, 6), (5, 6)},
        {(0, 1), (0, 6), (7, 1), (7, 6), (1, 0), (6, 0), (1, 7), (6, 7)},
        {(1, 1), (6, 6), (1, 6), (6, 1)}
    };
}

class CalMachine
{
public:
    // constructor
    CalMachine();

    // deconstrictor
    ~CalMachine();

    // check valid position
    static int32_t valid(int8_t player, int32_t x, int32_t y,
                         int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

    // execcute
    static int32_t execMove(int8_t player, int32_t x, int32_t y,
                            int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

    // have valid position
    static int32_t haveValidMoves(int8_t player,
                            int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

    // get valid position
    static int32_t getValidMoves(int8_t player, std::vector<NS_CALMACHINE::Coordinate> &moves,
                            int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

    // get valid porision (priority table)
    static int32_t getPriorityValidMoves(int8_t player, std::vector<NS_CALMACHINE::Coordinate> &moves,
                            int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

    // deep cocy
    static void deepCopy(int8_t src[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE],
                            int8_t dst[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

};

#endif // CALMACHINE_H
