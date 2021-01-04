#include "CalibrateWindow.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CalibrateWindow w;
    w.show();
    return a.exec();
}
