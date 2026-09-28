#include "maindialog.hpp"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MainDialog w;
//    w.show(); /// Start with main window hidden
    return QApplication::exec();
}
