#include "MiniAlphaGoMgr.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MiniAlphaGoMgr::getInstance().init();

    return a.exec();
}
