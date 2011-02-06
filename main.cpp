#include <QtGui/QApplication>
#include "headers/mediasitedownloder.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MediaSiteDownloder w;
    w.show();

    return a.exec();
}
