#include "CommAssistant.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    CCommAssistant w;
    w.show();
    return a.exec();
}
