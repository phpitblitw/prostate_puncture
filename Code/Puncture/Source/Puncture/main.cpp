#include "PunctureWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    PunctureWindow w;
    w.show();
    return a.exec();
}
