#ifndef MEDISITEDOWNLODER_H
#define MEDISITEDOWNLODER_H

#include <QMainWindow>

namespace Ui {
    class MediSiteDownloder;
}

class MediSiteDownloder : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediSiteDownloder(QWidget *parent = 0);
    ~MediSiteDownloder();

private:
    Ui::MediSiteDownloder *ui;
};

#endif // MEDISITEDOWNLODER_H
