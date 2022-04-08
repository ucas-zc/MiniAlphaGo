#include "SearchMgr.h"

// constructor
SearchMgr::SearchMgr()
{
    m_max_depth = 0;
    m_steps = 0;
    m_step_limit = 64;
    m_C = 1.414;
    m_start_time = time(nullptr);
    m_mct = nullptr;
}

// deconstructor
SearchMgr::~SearchMgr()
{
    if (m_mct != nullptr) {
        delete m_mct;
        m_mct = nullptr;
    }
}

// update mct
int32_t SearchMgr::updateMct(int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    // delete m_mct
    if (m_mct != nullptr) {
        delete m_mct;
        m_mct = nullptr;
    }

    // realloc
    m_mct = new Mct(MiniAlphaGoBoard::getInstance().whoFirst(), 0, board, nullptr, nullptr);

    m_max_depth = 0;
    m_steps = 0;

    // update m_steps
    for (int32_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            if (board[idx][idy] == NS_CALMACHINE::WHITE_CHESS
                    || board[idx][idy] == NS_CALMACHINE::BLACK_CHESS)
            {
                ++m_steps;
            }
        }
    }

    return RET::SUC;
}

// mcts
NS_CALMACHINE::Coordinate * SearchMgr::MCTS()
{
    if (m_mct == nullptr) {
        return nullptr;
    }

    m_max_depth = 0;
    m_start_time = time(nullptr);

    // create mct
    createMct(m_mct);

    return m_mct->m_child.empty() ? nullptr : getBestChild(m_mct, 0)->m_pre;
}

// select
Mct *SearchMgr::selectV(Mct *&node)
{
    if (node == nullptr) {
        return nullptr;
    }

    Mct *pNode = node;
    while (pNode != nullptr && RET::SUC != pNode->isTerminal())
    {
        if (RET::SUC == pNode->isFullExpand())
        {
            pNode = getBestChild(pNode, m_C);
        }
        else
        {
            return expand(pNode);
        }
    }

    return pNode;
}

// expand
Mct *SearchMgr::expand(Mct *&node)
{
    if (node == nullptr) {
        return nullptr;
    }

    int32_t choice_index = rand() % node->m_remain_valid_moves.size();
    node->addChild(node->m_remain_valid_moves[choice_index]);

    if (node->m_child[node->m_child.size() - 1]->m_depth > m_max_depth)
    {
        m_max_depth = node->m_child[node->m_child.size() - 1]->m_depth;
    }

    return node->m_child[node->m_child.size() - 1];
}

// simulate
int32_t SearchMgr::simulate(Mct *&node)
{
    if (node == nullptr) {
        return 0;
    }

    int32_t count = 0;
    while (time(nullptr) - m_start_time < m_time_limit)
    {
        // select
        Mct *pNode = selectV(node);
        int32_t reward = simulatePolicy(pNode);
        backPropagate(pNode, reward);

        ++count;
    }

    return count;
}

// simulate policy
int32_t SearchMgr::simulatePolicy(Mct *&node)
{
    if (node == nullptr) {
        return 0;
    }

    int32_t _step = 0;
    int8_t _player = node->m_player;
    int8_t _board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE];
    CalMachine::deepCopy(node->m_board, _board);

    while (_step + this->m_steps < this->m_step_limit)
    {
        std::vector<NS_CALMACHINE::Coordinate> _valid_set;
        if (_step + this->m_steps < NS_CALMACHINE::BOARD_SIZE * (NS_CALMACHINE::BOARD_SIZE - 1))
        {
            CalMachine::getPriorityValidMoves(_player, _valid_set, _board);
        }
        else
        {
            CalMachine::getValidMoves(_player, _valid_set, _board);
        }

        // switch player
        _player = 1 - _player;
        ++_step;

        if (_valid_set.empty()) {
            continue;
        }

        NS_CALMACHINE::Coordinate set = _valid_set[rand() % _valid_set.size()];
        CalMachine::execMove(_player, set.CoorX, set.CoorY, _board);
    }

    return dumbScore(MiniAlphaGoBoard::getInstance().whoFirst(), _board) > 0;
}

// back propagate
void SearchMgr::backPropagate(Mct *&node, int32_t reward)
{
    if (node == nullptr) {
        return;
    }

    Mct *pV = node;
    while (pV != nullptr) {
        ++(pV->m_N);
        pV->m_Q += pV->m_player == MiniAlphaGoBoard::getInstance().whoFirst() ?
                    reward : (1 - reward);

        pV = pV->m_parent;
    }
}

// create mct
int32_t SearchMgr::createMct(Mct *&root)
{
    int32_t count = 0;
    m_time_limit = 10;
    while (time(nullptr) - m_start_time < m_time_limit) {
        // select
        Mct *pNode = selectV(root);

        // simulate
        int32_t reward = simulatePolicy(pNode);

        // back propagate
        backPropagate(pNode, reward);

        ++count;

        if (RET::SUC == root->isFullExpand())
        {
            break;
        }
    }

    if (root->m_child.empty())
    {
        return count;
    }

    for (int32_t idx = 0; idx < root->m_child.size(); ++idx)
    {
        simulate(root->m_child[idx]);
    }

    return count;
}

// get a best children
Mct *SearchMgr::getBestChild(Mct *root, float C)
{
    // valid check
    if (root == nullptr) {
        return nullptr;
    }

    int32_t max_index = UCB(root, C);
    return max_index == - 1 ? nullptr : root->m_child[max_index];
}

// compute
int32_t SearchMgr::UCB(Mct *node, float C)
{
    // valid check
    if (node == nullptr) {
        return -1;
    }

    float max_ucb = 0;
    int32_t max_index = 0;
    std::vector<Mct*>::iterator iter = node->m_child.begin();
    for (; iter != node->m_child.end(); ++iter)
    {
        float ucb = 1 - (*iter)->m_Q / (*iter)->m_N
                + C * sqrt(log(node->m_N) / (*iter)->m_N);
        if (max_ucb < ucb)
        {
            max_ucb = ucb;
            max_index = iter - node->m_child.begin();
        }
    }

    return max_index;
}

// dumb score
int32_t SearchMgr::dumbScore(int8_t player,
                             int8_t board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    int32_t score = 0;

    for (int32_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            if (board[idx][idy] == player)
            {
                ++score;
            }
            else if (board[idx][idy] == 1 - player)
            {
                --score;
            }
        }
    }

    return score;
}
