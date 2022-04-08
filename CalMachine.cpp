#include "CalMachine.h"

// constructor
CalMachine::CalMachine()
{
}

// deconstructor
CalMachine::~CalMachine()
{}

// valid check
int32_t CalMachine::valid(int8_t player, int32_t x, int32_t y,
                          int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    if (x < 0 || x >= NS_CALMACHINE::BOARD_SIZE
            || y < 0 || y >= NS_CALMACHINE::BOARD_SIZE)
    {
        return RET::FAIL;
    }

    // invalid if board[x][y] is not null
    if (NS_CALMACHINE::NULL_CHESS != board[x][y])
    {
        return RET::FAIL;
    }

    // right
    if (y < NS_CALMACHINE::BOARD_SIZE - 2 && board[x][y + 1] == 1 - player)
    {
        for (int32_t idy = y + 2; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            if (board[x][idy] == player)
            {
                return RET::SUC;
            }
            else if (board[x][idy] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // left
    if (y > 1 && board[x][y - 1] == 1 - player)
    {
        for (int32_t idy = y - 2; idy >= 0; --idy)
        {
            if (board[x][idy] == player)
            {
                return RET::SUC;
            }
            else if (board[x][idy] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // up
    if (x > 1 && board[x -1][y] == 1 - player)
    {
        for (int32_t idx = x - 2;  idx >= 0; --idx)
        {
            if (board[idx][y] == player)
            {
                return RET::SUC;
            }
            else if (board[idx][y] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // down
    if (x < NS_CALMACHINE::BOARD_SIZE - 1 && board[x + 1][y] == 1 - player)
    {
        for (int32_t idx = x + 2; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
        {
            if (board[idx][y] == player)
            {
                return RET::SUC;
            }
            else if (board[idx][y] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // right & up
    if (x > 1 && y < NS_CALMACHINE::BOARD_SIZE - 2
            && board[x - 1][y + 1] == 1 - player)
    {
        int32_t min = (x + 1) < (NS_CALMACHINE::BOARD_SIZE - y) ?
                    (x + 1) : (NS_CALMACHINE::BOARD_SIZE - y);
        for (int32_t idx = 2; idx < min; ++idx)
        {
            if (board[x - idx][y + idx] == player)
            {
                return RET::SUC;
            }
            else if (board[x - idx][y + idx] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // right & down
    if (x < NS_CALMACHINE::BOARD_SIZE - 2 && y < NS_CALMACHINE::BOARD_SIZE - 2
            && board[x + 1][y + 1] == 1 - player)
    {
        int32_t min = (NS_CALMACHINE::BOARD_SIZE - x) < (NS_CALMACHINE::BOARD_SIZE - y)
                ? (NS_CALMACHINE::BOARD_SIZE - x) : (NS_CALMACHINE::BOARD_SIZE - y);
        for (int32_t idx = 2; idx < min; ++idx)
        {
            if (board[x + idx][y + idx] == player)
            {
                return RET::SUC;
            }
            else if (board[x + idx][y + idx] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // left & up
    if (x > 1 && y > 1 && board[x - 1][y - 1] == 1 - player)
    {
        int32_t min = (x + 1) < (y + 1) ? (x + 1) : (y + 1);
        for (int32_t idx = 2; idx < min; ++idx)
        {
            if (board[x - idx][y - idx] == player)
            {
                return RET::SUC;
            }
            else if (board[x - idx][y - idx] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // left & down
    if (x < NS_CALMACHINE::BOARD_SIZE - 2 && y > 1
            && board[x + 1][y - 1] == 1 - player)
    {
        int32_t min = (NS_CALMACHINE::BOARD_SIZE - x) < (y + 1)
                ? (NS_CALMACHINE::BOARD_SIZE - x) : (y + 1);
        for (int32_t idx = 2; idx < min; ++idx)
        {
            if (board[x + idx][y - idx] == player)
            {
                return RET::SUC;
            }
            else if (board[x + idx][y - idx] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }

    return RET::FAIL;
}

int32_t CalMachine::execMove(int8_t player, int32_t x, int32_t y,
                             int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    // valid check
    if (x < 0 || x >= NS_CALMACHINE::BOARD_SIZE
            || y < 0 || y >= NS_CALMACHINE::BOARD_SIZE)
    {
        return RET::FAIL;
    }

    // play chess
    board[x][y] = player;

    // right
    if (y < NS_CALMACHINE::BOARD_SIZE - 2
            && board[x][y + 1] == 1 - player)
    {
        for (int32_t idy = y + 2; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            if (board[x][idy] == player)
            {
                for (int32_t idx = y + 1; idx < idy; ++idx)
                {
                    board[x][idx] = player;
                }

                break;
            }
            else if (board[x][idy] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // left
    if (y > 1 && board[x][y - 1] == 1 - player)
    {
        for (int32_t idy = y - 2; idy >= 0; --idy)
        {
            if (board[x][idy] == player)
            {
                for (int32_t idx = y - 1; idx > idy; --idx)
                {
                    board[x][idx] = player;
                }

                break;
            }
            else if (board[x][idy] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // up
    if (x > 1 && board[x - 1][y] == 1 - player)
    {
        for (int32_t idx = x - 2; idx >= 0; --idx)
        {
            if (board[idx][y] == player)
            {
                for (int32_t idy = x - 1; idy > idx; --idy)
                {
                    board[idy][y] = player;
                }

                break;
            }
            else if (board[idx][y] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // down
    if (x < NS_CALMACHINE::BOARD_SIZE - 2 && board[x + 1][y] == 1 - player)
    {
        for (int32_t idx = x + 2; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
        {
            if (board[idx][y] == player)
            {
                for (int32_t idy = x + 1; idy < idx; ++idy)
                {
                    board[idy][y] = player;
                }

                break;
            }
            else if (board[idx][y] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // right & up
    if (x > 1 && y <NS_CALMACHINE::BOARD_SIZE - 2
            && board[x - 1][y + 1] == 1 - player)
    {
        int32_t min = (x + 1) < (NS_CALMACHINE::BOARD_SIZE - y) ?
                    (x + 1) : (NS_CALMACHINE::BOARD_SIZE - y);
        for (int32_t idx = 2; idx < min; ++idx)
        {
            if (board[x - idx][y + idx] == player)
            {
                for (int32_t idy = 1; idy < idx; ++idy)
                {
                    board[x - idy][y + idy] = player;
                }

                break;
            }
            else if (board[x - idx][y + idx] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // right & down
    if (x < NS_CALMACHINE::BOARD_SIZE - 2 && y < NS_CALMACHINE::BOARD_SIZE - 2
            && board[x + 1][y + 1] == 1 - player)
    {
        int32_t min = (NS_CALMACHINE::BOARD_SIZE - x) < (NS_CALMACHINE::BOARD_SIZE - y)
                ? (NS_CALMACHINE::BOARD_SIZE - x) : (NS_CALMACHINE::BOARD_SIZE - y);
        for (int32_t idx = 2; idx < min; ++idx)
        {
            if (board[x + idx][y + idx] == player)
            {
                for (int32_t idy = 1; idy < idx; ++idy)
                {
                    board[x + idy][y + idy] = player;
                }

                break;
            }
            else if (board[x + idx][y + idx] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // left & up
    if (x > 1 && y > 1 && board[x - 1][y - 1] == 1 - player)
    {
        int32_t min = (x + 1) < (y + 1) ? (x + 1) : (y + 1);
        for (int32_t idx = 2; idx < min; ++idx)
        {
            if (board[x - idx][y - idx] == player)
            {
                for (int32_t idy = 1; idy < idx; ++idy)
                {
                    board[x - idy][y - idy] = player;
                }

                break;
            }
            else if (board[x - idx][y - idx] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }
    // left & down
    if (x < NS_CALMACHINE::BOARD_SIZE - 2 && y > 1
            && board[x + 1][y - 1] == 1 - player)
    {
        int32_t min = (NS_CALMACHINE::BOARD_SIZE - x) < (y + 1)
                ? (NS_CALMACHINE::BOARD_SIZE - x) : (y + 1);
        for (int32_t idx = 2; idx < min; ++idx)
        {
            if (board[x + idx][y - idx] == player)
            {
                for (int32_t idy = 1; idy < idx; ++idy)
                {
                    board[x + idy][y - idy] = player;
                }

                break;
            }
            else if (board[x + idx][y - idx] == NS_CALMACHINE::NULL_CHESS)
            {
                break;
            }
        }
    }

    return RET::FAIL;
}

// return success if player have valid moves
int32_t CalMachine::haveValidMoves(int8_t player,
                                   int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    for (int32_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            if (RET::SUC == CalMachine::valid(player, idx, idy, board))
            {
                return RET::SUC;
            }
        }
    }

    return RET::FAIL;
}

// return moves if player have valid moves
int32_t CalMachine::getValidMoves(int8_t player, std::vector<NS_CALMACHINE::Coordinate> &moves,
                                  int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    moves.clear();

    for (int32_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            if (RET::SUC == CalMachine::valid(player, idx, idy, board))
            {
                NS_CALMACHINE::Coordinate move;
                move.CoorX = idx;
                move.CoorY = idy;

                moves.push_back(move);
            }
        }
    }

    return moves.size() == 0 ? RET::FAIL : RET::SUC;
}

// return priority move if player have valid moves
int32_t CalMachine::getPriorityValidMoves(int8_t player, std::vector<NS_CALMACHINE::Coordinate> &moves,
                                          int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    moves.clear();

    for (int32_t idx = 0; idx < NS_CALMACHINE::PRIORITY_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            if (RET::SUC == CalMachine::valid(player, NS_CALMACHINE::priority_table[idx].Coor[idy].CoorX,
                                              NS_CALMACHINE::priority_table[idx].Coor[idy].CoorY, board))
            {
                NS_CALMACHINE::Coordinate move;
                move.CoorX = NS_CALMACHINE::priority_table[idx].Coor[idy].CoorX;
                move.CoorY = NS_CALMACHINE::priority_table[idx].Coor[idy].CoorY;

                moves.push_back(move);
            }
        }
    }

    return moves.size() == 0 ? RET::FAIL : RET::SUC;
}

// deep copy
void CalMachine::deepCopy(int8_t src[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE],
                          int8_t dst[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    for (int32_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            dst[idx][idy] = src[idx][idy];
        }
    }
}
