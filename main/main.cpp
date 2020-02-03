#include "mainFrame.h"
#include "Settings.h"
#include <QtWidgets/QApplication>

int main(int argc, char *argv[])
{
    Settings::ensureConfig();

    QApplication a(argc, argv);
    mainFrame w;
    w.show();
    return a.exec();
}
