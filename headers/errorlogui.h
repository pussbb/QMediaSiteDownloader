#ifndef ERRORLOGUI_H
#define ERRORLOGUI_H

#include <QDialog>

namespace Ui {
    class ErrorLogUi;
}

class ErrorLogUi : public QDialog
{
    Q_OBJECT

public:
    explicit ErrorLogUi(QWidget *parent = 0);
    ~ErrorLogUi();
    void build_list(QStringList list);

private slots:
    void on_closeuilog_clicked();

private:
    Ui::ErrorLogUi *ui;
};

#endif // ERRORLOGUI_H
