#ifndef SEARCHMGR_H
#define SEARCHMGR_H

#include "CalMachine.h"
#include "MiniAlphaGoBoard.h"
#include "Mct.h"
#include <time.h>
#include <math.h>
#include <cstdio>
#include <cstdint>

class SearchMgr
{
public:
    // get instance
    static SearchMgr &getInstance()
    {
        static SearchMgr _instance;
        return _instance;
    }

    // deconstructor
    ~SearchMgr();

    // update mct
    int32_t updateMct(int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

    // mcts
    NS_CALMACHINE::Coordinate *MCTS();

private:
    // constructor
    SearchMgr();

    // create mct
    int32_t createMct(Mct *&root);

    // select a node to expand
    Mct *selectV(Mct *&node);

    // expand
    Mct *expand(Mct *&node);

    // simulate
    int32_t simulate(Mct *&node);

    //simulate policy
    int32_t simulatePolicy(Mct *&node);

    //back
    void backPropagate(Mct *&node, int32_t reward);

    // compute score
    int32_t dumbScore(int8_t player, int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

    // get best children
    Mct *getBestChild(Mct *root, float C);

    // compute UCB
    int32_t UCB(Mct *node, float C);

 private:
    // max depth
    int32_t m_max_depth;

    // step
    int32_t m_steps;

    // limit of step
    int32_t m_step_limit;

    // C
    float m_C;

    // start time of single step
    time_t m_start_time;

    // limit time of single step
    time_t m_time_limit;

    // root node of mct
    Mct *m_mct;
};

#endif // SEARCHMGR_H
