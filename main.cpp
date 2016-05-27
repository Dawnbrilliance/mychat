#include "chatdialog.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    chatDialog w;
    w.show();

    return a.exec();
}
