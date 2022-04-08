#include "MiniAlphaGoUi.h"
#include "MiniAlphaGoBoard.h"
#include "MiniAlphaGoMgr.h"

// constructor
MiniAlphaGoUi::MiniAlphaGoUi(QWidget *parent) :
    QMainWindow(parent)
{
    this->createGameUi();
}

// deconstructor
MiniAlphaGoUi::~MiniAlphaGoUi()
{
    if (name != nullptr) {
        delete name;
        name = nullptr;
    }

    if (player_first != nullptr) {
        delete player_first;
        player_first = nullptr;
    }

    if (ai_first != nullptr) {
        delete ai_first;
        ai_first = nullptr;
    }
}

// create game ui
void MiniAlphaGoUi::createGameUi()
{
    // set window title
    setWindowTitle("Mini Alpha Go");

    // set background color
    QPalette pal = palette();
    pal.setColor(QPalette::Background, Qt::black);
    setPalette(pal);

    // set name
    name = new QLabel(this);
    name->setText("MiniAlphaGo");
    name->setGeometry(200, 50, 350, 100);
    name->setStyleSheet("color:white; font: 20px; background: red");
    name->setAlignment(Qt::AlignCenter);

    // player first(button)
    player_first = new QPushButton("PLAYER FIRST", this);
    player_first->setGeometry(250, 250, 250, 70);
    player_first->setStyleSheet("color: black; font: 25px; background: white");
    QObject::connect(player_first, &QPushButton::clicked, this, &MiniAlphaGoUi::playerFirst);

    // ai first(button)
    ai_first = new QPushButton("AI FIRST", this);
    ai_first->setGeometry(250, 350, 250, 70);
    ai_first->setStyleSheet("color: black; font: 25px; background:white");
    QObject::connect(ai_first, &QPushButton::clicked, this, &MiniAlphaGoUi::aiFirst);
}

// player first
void MiniAlphaGoUi::playerFirst()
{
    // close window
    close();

    // switch first
    MiniAlphaGoBoard::getInstance().switchFirst(NS_MAGBOARD::PLAYER_FIRST);

    // init start time of single step
    MiniAlphaGoMgr::getInstance().initSingleTime();

    // show board
    MiniAlphaGoBoard::getInstance().show();
}

// ai first
void MiniAlphaGoUi::aiFirst()
{
    // close window
    close();

    // switch first
    MiniAlphaGoBoard::getInstance().switchFirst(NS_MAGBOARD::AI_FIRST);

    // init start time of single step
    MiniAlphaGoMgr::getInstance().initSingleTime();

    // show board
    MiniAlphaGoBoard::getInstance().show();

    // ai play chess
    MiniAlphaGoMgr::getInstance().updateBoard(NS_MAGBOARD::AI_FIRST);
}
