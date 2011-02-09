#ifndef ADDTASK_H
#define ADDTASK_H

#include <QDialog>

namespace Ui {
    class AddTask;
}

class AddTask : public QDialog
{
    Q_OBJECT

public:
    explicit AddTask(QWidget *parent = 0);
    ~AddTask();

private slots:
    void on_toolButton_clicked();

private:
    Ui::AddTask *ui;
};

#endif // ADDTASK_H
