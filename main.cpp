#include "MS_VS_GAS.h"
#include <QFile>
#include <QtWidgets/QApplication>


int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MS_VS_GAS w;

    w.show();
    return a.exec();
}
