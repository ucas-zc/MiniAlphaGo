#ifndef MINIALPHAGOMGR_H
#define MINIALPHAGOMGR_H

#include "ReturnCode.h"
#include "MiniAlphaGoUi.h"
#include "MiniAlphaGoBoard.h"
#include "SearchMgr.h"
#include "CalMachine.h"
#include <vector>

class MiniAlphaGoMgr
{
public:
    /**
      *@brief get instance
      *@return instance
      **/
    static MiniAlphaGoMgr &getInstance()
    {
        static MiniAlphaGoMgr _instance;
        return _instance;
    }

    /**
      *@brief deconstructor
      **/
    ~MiniAlphaGoMgr();

    // init
    int32_t init();

    // play chess
    // (x, y) is coordinate
    int32_t playChess(int8_t player, int32_t x, int32_t y);

    // init single time
    void initSingleTime();

    // update board
    int32_t updateBoard(int8_t player);

private:
    /**
      *brief constructo
      **/
    MiniAlphaGoMgr();

    // update board
    //int32_t updateBoard(int8_t player);

private:
    // board
    int8_t m_board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE];

    //single time
    time_t m_single_start_time;
};


#endif // MINIALPHAGOMGR_H
