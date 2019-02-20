#include "serverwidget.h"
#include <QApplication>
#include "cliwidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    ServerWidget w;
    w.show();
    CliWidget w1;
    w1.show();

    return a.exec();
}
