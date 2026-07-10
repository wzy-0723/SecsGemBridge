#include "clientwidget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Clientwidget w;
    w.show();
    return QApplication::exec();
}
