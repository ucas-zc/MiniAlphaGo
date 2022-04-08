#include "MiniAlphaGoMgr.h"

MiniAlphaGoMgr::MiniAlphaGoMgr()
{
    for (int32_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            m_board[idx][idy] = NS_CALMACHINE::NULL_CHESS;
        }
    }

    int32_t left_pos = (NS_CALMACHINE::BOARD_SIZE - 1) / 2;
    int32_t right_pos = NS_CALMACHINE::BOARD_SIZE / 2;
    m_board[left_pos][left_pos] = NS_CALMACHINE::BLACK_CHESS;
    m_board[left_pos][right_pos] = NS_CALMACHINE::WHITE_CHESS;
    m_board[right_pos][left_pos] = NS_CALMACHINE::WHITE_CHESS;
    m_board[right_pos][right_pos] = NS_CALMACHINE::BLACK_CHESS;

    m_single_start_time = time(nullptr);

    SearchMgr::getInstance().updateMct(m_board);
}

MiniAlphaGoMgr::~MiniAlphaGoMgr()
{}

// init
int32_t MiniAlphaGoMgr::init()
{
    MiniAlphaGoUi::getInstance().show();

    return RET::SUC;
}

// init start time of single step
void MiniAlphaGoMgr::initSingleTime()
{
    m_single_start_time = time(nullptr);
}

// play chess
int32_t MiniAlphaGoMgr::playChess(int8_t player, int32_t x, int32_t y)
{
    if (RET::SUC == CalMachine::valid(player, x, y, m_board))
    {
        CalMachine::execMove(player, x, y, m_board);

        MiniAlphaGoBoard::getInstance().computeTime(m_single_start_time);

        m_single_start_time = time(nullptr);

        MiniAlphaGoBoard::getInstance().switchFirst((1 - player));

        SearchMgr::getInstance().updateMct(m_board);

        MiniAlphaGoMgr::getInstance().updateBoard(1 - player);

        return RET::SUC;
    }

    return RET::FAIL;
}

// update board
int32_t MiniAlphaGoMgr::updateBoard(int8_t player)
{
    //update ui
    MiniAlphaGoBoard::getInstance().updateBoard(m_board);

    // ai first
    if (NS_MAGBOARD::AI_FIRST == player)
    {
        // exec if ai have valid moves
        if (RET::SUC == CalMachine::haveValidMoves(player, m_board))
        {
            // mcts
            NS_CALMACHINE::Coordinate *pCoor = SearchMgr::getInstance().MCTS();

            if (pCoor == nullptr) {
                MiniAlphaGoBoard::getInstance().switchFirst(1 - player);
            } else {
                CalMachine::execMove(player, pCoor->CoorX, pCoor->CoorY, m_board);

                // compute time
                MiniAlphaGoBoard::getInstance().computeTime(m_single_start_time);

                // update start time of next step
                m_single_start_time = time(nullptr);

                // switch player
                MiniAlphaGoBoard::getInstance().switchFirst(1 - player);

                // update board
                MiniAlphaGoMgr::getInstance().updateBoard(1 - player);
            }
        } else {
            // over if player don't have valid moves
            if (RET::SUC != CalMachine::haveValidMoves(1 - player, m_board))
            {
                MiniAlphaGoBoard::getInstance().outputResult();

                return RET::SUC;
            }

            MiniAlphaGoBoard::getInstance().switchFirst(1 - player);

            return RET::SUC;
        }
    }
    // player first
    {
        if (RET::SUC != CalMachine::haveValidMoves(player, m_board))
        {
            // over if ai and player don't have valid moves
            if (RET::SUC != CalMachine::haveValidMoves(1 - player, m_board))
            {
                MiniAlphaGoBoard::getInstance().outputResult();

                return RET::SUC;
            }

            // switch first
            MiniAlphaGoBoard::getInstance().switchFirst(1 - player);

            // update board
            MiniAlphaGoMgr::getInstance().updateBoard(1 - player);

            return  RET::SUC;
        }
    }

    return RET::FAIL;
}
