#ifndef MEDIASITEDOWNLODER_H
#define MEDIASITEDOWNLODER_H

#include <QMainWindow>

namespace Ui {
    class MediaSiteDownloder;
}

class MediaSiteDownloder : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaSiteDownloder(QWidget *parent = 0);
    ~MediaSiteDownloder();

private slots:
    void on_actionExit_triggered();

    void on_actionAbout_QT_triggered();

private:
    Ui::MediaSiteDownloder *ui;
};

#endif // MEDISITEDOWNLODER_H
