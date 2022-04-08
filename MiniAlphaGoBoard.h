#ifndef MINIALPHAGOBOARD_H
#define MINIALPHAGOBOARD_H

#include <QMainWindow>
#include <QPushButton>
#include <QMouseEvent>
#include <QPalette>
#include <QLabel>
#include <QMessageBox>
#include <QPainter>
#include <cstdint>
#include <cstdio>
#include <time.h>
//#include "MiniAlphaGoMgr.h"
#include "CalMachine.h"

namespace NS_MAGBOARD
{
    // ai first
    static const int8_t AI_FIRST = 0;
    // player first
    static const int8_t PLAYER_FIRST = 1;
    // board coordinate
    static const int32_t BOARD_X = 100;
    static const int32_t BOARD_Y = 100;
    // single board size
    static const int32_t WIDTH = 50;
    // screen coordinate
    static const int32_t SCREEN_X = 110;
    static const int32_t SCREEN_Y = 100;
}

class MiniAlphaGoBoard : public QMainWindow
{
    Q_OBJECT

public:
    // get instance
    static MiniAlphaGoBoard &getInstance()
    {
        static MiniAlphaGoBoard _instance;
        return _instance;
    }

    // deconstructor
    ~MiniAlphaGoBoard();

    // switch player
    void switchFirst(int8_t first);

    // get player
    int8_t whoFirst();

    // update board
    void updateBoard(int8_t board_msg[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE]);

    // output final result
    void outputResult();

    // compute single time
    void computeTime(time_t start_time);

private:
    //constructor
    MiniAlphaGoBoard(QWidget *parent = nullptr);

    // paint event
    void paintEvent(QPaintEvent *);

    // paint board
    void paintBoard();

    // paint chess
    void paintChess();

    // paint time
    void paintTime();

    // output count of chess
    void outputCount();

    // mouse press event
    void mousePressEvent(QMouseEvent *event);

private:
    // first flag
    int8_t first;

    // number of black chess
    int32_t black_num;

    // number of white chess
    int32_t white_num;

    // board
    QPainter *board;

    // output message
    QMessageBox *msg;

    // board message
    int8_t m_board[NS_CALMACHINE::BOARD_SIZE][NS_CALMACHINE::BOARD_SIZE];

    // single time of ai player
    time_t ai_single_time;

    // total time of ai player
    time_t ai_total_time;

    // single time of player
    time_t player_single_time;

    // total time of player
    time_t player_total_time;

};

#endif // MINIALPHAGOBOARD_H
