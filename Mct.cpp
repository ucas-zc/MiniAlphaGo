#include "Mct.h"

// constructor
Mct::Mct(int8_t player, int32_t depth,
         int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE],
         Mct *parent, NS_CALMACHINE::Coordinate *pre)
{
    m_player = player;
    m_depth = depth;
    m_N = 0;
    m_Q = 0;
    m_parent = parent;

    m_pre = new NS_CALMACHINE::Coordinate();
    m_pre->CoorX = pre != nullptr ? pre->CoorX : -1;
    m_pre->CoorY = pre != nullptr ? pre->CoorY : -1;

    CalMachine::deepCopy(board, m_board);

    CalMachine::getValidMoves(m_player, m_remain_valid_moves, m_board);
}

// deconstructor
Mct::~Mct()
{}

// add children
void Mct::addChild(NS_CALMACHINE::Coordinate move)
{
    // delete this valid move
    removeValidMove(move);

    Mct *child = new Mct(1 - m_player, m_depth + 1, m_board, this, &move);

    CalMachine::execMove(m_player, move.CoorX, move.CoorY, child->m_board);

    // add children
    m_child.push_back(child);
}

// remove a valid move
void Mct::removeValidMove(NS_CALMACHINE::Coordinate move)
{
    std::vector<NS_CALMACHINE::Coordinate>::iterator iter = m_remain_valid_moves.begin();
    for (; iter != m_remain_valid_moves.end(); ++iter)
    {
        if (iter->CoorX == move.CoorX && iter->CoorY == move.CoorY)
        {
            m_remain_valid_moves.erase(iter);

            break;
        }
    }
}

// is full expand?
int32_t Mct::isFullExpand()
{
    return m_remain_valid_moves.empty() ? RET::SUC : RET::FAIL;
}

// is terminal
int32_t Mct::isTerminal()
{
    return m_remain_valid_moves.empty() && m_child.empty() ? RET::SUC : RET::FAIL;
}
