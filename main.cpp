#include <QtGui/QApplication>
#include "headers/medisitedownloder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MediSiteDownloder w;
    w.show();

    return a.exec();
}
