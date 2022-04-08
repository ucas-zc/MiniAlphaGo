#include "MiniAlphaGoBoard.h"
#include "MiniAlphaGoMgr.h"

// constructor
MiniAlphaGoBoard::MiniAlphaGoBoard(QWidget *parent)
{
    first = NS_MAGBOARD::AI_FIRST;

    black_num = 2;
    white_num = 2;

    board= new QPainter(this);
    msg = new QMessageBox(this);

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
    m_board[right_pos][left_pos] = NS_CALMACHINE::WHITE_CHESS;
    m_board[left_pos][right_pos] = NS_CALMACHINE::WHITE_CHESS;
    m_board[right_pos][right_pos] = NS_CALMACHINE::BLACK_CHESS;

    ai_single_time = 0;
    ai_total_time = 0;
    player_single_time = 0;
    player_total_time = 0;
}

// deconstructor
MiniAlphaGoBoard::~MiniAlphaGoBoard()
{
    if (board != nullptr) {
        delete board;
        board = nullptr;
    }

    if (msg != nullptr) {
        delete msg;
        msg = nullptr;
    }
}

// paint board
void MiniAlphaGoBoard::paintBoard()
{
    // set line style
    board->setPen(QPen(Qt::darkGreen, 2, Qt::SolidLine));

    for (int8_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE + 1; ++idx)
    {
        board->drawLine(NS_MAGBOARD::BOARD_X,
                         NS_MAGBOARD::BOARD_Y + NS_MAGBOARD::WIDTH * idx,
                         NS_MAGBOARD::BOARD_X + NS_MAGBOARD::WIDTH * NS_CALMACHINE::BOARD_SIZE,
                         NS_MAGBOARD::BOARD_Y + NS_MAGBOARD::WIDTH * idx);
    }

    for (int8_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE + 1; ++idx)
    {
        board->drawLine(NS_MAGBOARD::BOARD_X + NS_MAGBOARD::WIDTH * idx,
                        NS_MAGBOARD::BOARD_Y,
                        NS_MAGBOARD::BOARD_X + NS_MAGBOARD::WIDTH * idx,
                        NS_MAGBOARD::BOARD_Y + NS_MAGBOARD::WIDTH * NS_CALMACHINE::BOARD_SIZE);
    }
}

// paint chess
void MiniAlphaGoBoard::paintChess()
{
    for (int32_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            // paint white chess if m_board[x][y] = white
            if (m_board[idx][idy] == NS_CALMACHINE::WHITE_CHESS)
            {
                board->setPen(QPen(Qt::white, 2, Qt::SolidLine));
                board->setBrush(QBrush(Qt::white, Qt::SolidPattern));
                board->drawEllipse(NS_MAGBOARD::BOARD_Y + idy * NS_MAGBOARD::WIDTH,
                                   NS_MAGBOARD::BOARD_X + idx * NS_MAGBOARD::WIDTH,
                                   NS_MAGBOARD::WIDTH, NS_MAGBOARD::WIDTH);
            }
            // paint black chess if m_board[x][y] = black
            else if (m_board[idx][idy] == NS_CALMACHINE::BLACK_CHESS)
            {
                board->setPen(QPen(Qt::black, 2, Qt::SolidLine));
                board->setBrush(QBrush(Qt::black, Qt::SolidPattern));
                board->drawEllipse(NS_MAGBOARD::BOARD_Y + idy * NS_MAGBOARD::WIDTH,
                                   NS_MAGBOARD::BOARD_X + idx * NS_MAGBOARD::WIDTH,
                                   NS_MAGBOARD::WIDTH, NS_MAGBOARD::WIDTH);
            }
        }
    }
}

// paint time
void MiniAlphaGoBoard::paintTime()
{
    // single time & total time (ai)
    std::string ai_time = std::to_string(ai_single_time)
            + " : " + std::to_string(ai_total_time);
    board->setPen(QPen(Qt::black, 2 , Qt::SolidLine));
    board->drawText(50, 90, ai_time.c_str());

    // single time & total time (player)
    std::string player_time = std::to_string(player_single_time)
            + " : " + std::to_string(player_total_time);
    board->setPen((QPen(Qt::black, 2, Qt::SolidLine)));
    board->drawText(530, 90, player_time.c_str());
}

// output count
void MiniAlphaGoBoard::outputCount()
{
    // number of black chess
    std::string black_count = std::to_string(black_num);
    // set style
    board->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    board->setBrush(QBrush(Qt::black, Qt::SolidPattern));
    // paint black chess
    board->drawEllipse(40, 20, NS_MAGBOARD::WIDTH, NS_MAGBOARD::WIDTH);
    // output number
    board->setPen(QPen(Qt::white, 2, Qt::SolidLine));
    board->drawText(60, 50, black_count.c_str());

    // number of white chess
    std::string white_count = std::to_string(white_num);
    // set style
    board->setPen((QPen(Qt::white, 2, Qt::SolidLine)));
    board->setBrush(QBrush(Qt::white, Qt::SolidPattern));
    // paint white chess
    board->drawEllipse(520, 20, NS_MAGBOARD::WIDTH, NS_MAGBOARD::WIDTH);
    // output number
    board->setPen(QPen(Qt::black, 2, Qt::SolidLine));
    board->drawText(540, 50, white_count.c_str());
}

// paint event
void MiniAlphaGoBoard::paintEvent(QPaintEvent *)
{
    if (board == nullptr) {
        return;
    }

    // set window title
    if (first == NS_MAGBOARD::AI_FIRST)
    {
        setWindowTitle("Mini Alpha Go(AI FIRST)");
    }
    else if(first == NS_MAGBOARD::PLAYER_FIRST)
    {
        setWindowTitle("Mini Alpha Go(PLAYER FIRST)");
    }

    // set background color
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::gray);
    setPalette(pal);

    // begin
    board->begin(this);

    // paint board(8*8)
    paintBoard();

    // paint chess
    paintChess();

    // paint time
    paintTime();

    // output number of chess
    outputCount();

    board->end();
}

// mouse press event
void MiniAlphaGoBoard::mousePressEvent(QMouseEvent *event)
{
    if (first != NS_MAGBOARD::PLAYER_FIRST)
    {
        if (msg == nullptr) {
            return;
        }

        msg->setWindowTitle("Warning!");
        msg->setText("No right to play chess!");
        msg->show();
        return;
    }

    // get coordinate
    int32_t _x = event->globalX() - NS_MAGBOARD::BOARD_X - NS_MAGBOARD::SCREEN_X;
    int32_t _y = event->globalY() - NS_MAGBOARD::BOARD_Y - NS_MAGBOARD::SCREEN_Y;

    if ((_x >= 0 && _x < NS_CALMACHINE::BOARD_SIZE * NS_MAGBOARD::WIDTH)
            && (_y >= 0 && _y < NS_CALMACHINE::BOARD_SIZE * NS_MAGBOARD::WIDTH))
    {
        _x = _x / NS_MAGBOARD::WIDTH;
        _y = _y / NS_MAGBOARD::WIDTH;

        if (first == NS_MAGBOARD::PLAYER_FIRST)
        {
            MiniAlphaGoMgr::getInstance().playChess(first, _y, _x);
        }
    }
}

// switch first
void MiniAlphaGoBoard::switchFirst(int8_t first)
{
    this->first = first;
}

// get first
int8_t MiniAlphaGoBoard::whoFirst()
{
    return this->first;
}

// update board
void MiniAlphaGoBoard::updateBoard(int8_t board_msg[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE])
{
    white_num = black_num = 0;

    for (int32_t idx = 0; idx < NS_CALMACHINE::BOARD_SIZE; ++idx)
    {
        for (int32_t idy = 0; idy < NS_CALMACHINE::BOARD_SIZE; ++idy)
        {
            //copy board message
            m_board[idx][idy] = board_msg[idx][idy];

            // compute number of black chess
            black_num = m_board[idx][idy] == NS_CALMACHINE::BLACK_CHESS ? ++black_num : black_num;

            // compute number of white chess
            white_num = m_board[idx][idy] == NS_CALMACHINE::WHITE_CHESS ? ++white_num : white_num;
        }
    }

    // repaint
    repaint();
}

// output result
void MiniAlphaGoBoard::outputResult()
{
    if (msg == nullptr)
    {
        return;
    }

    msg->setWindowTitle("Result");

    if (black_num > white_num) {
        msg->setText("You Lose!");
    }
    else if (black_num < white_num) {
        msg->setText("You Win!");
    }
    else {
        msg->setText("Tie!");
    }

    msg->show();
}

// compute time
void MiniAlphaGoBoard::computeTime(time_t start_time)
{
    // compute time (ai)
    if (first == NS_MAGBOARD::AI_FIRST)
    {
        ai_single_time = time(nullptr) - start_time;
        ai_total_time += ai_single_time;
    }
    else if (first == NS_MAGBOARD::PLAYER_FIRST) {
        player_single_time = time(nullptr) - start_time;
        player_total_time += player_single_time;
    }
}
