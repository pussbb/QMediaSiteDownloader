#include <QtGui/QApplication>
#include "headers/mediasitedownloder.h"
#include "QSettings"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QCoreApplication::setOrganizationName("_pussbb");
    QCoreApplication::setOrganizationDomain("krabik.co.ua");
    QCoreApplication::setApplicationName("QMSD");
    QApplication::setApplicationName("QMSD");
    QApplication::setApplicationVersion("0.0.0.1");
    MediaSiteDownloder w;
    w.show();

    return a.exec();
}
