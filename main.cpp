#include "lookthesky.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setFont(QFont(":/assets/fonts/SFD.ttf"));
    LookTheSky w;
    w.show();
    return a.exec();
}
