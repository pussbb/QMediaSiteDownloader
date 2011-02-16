#ifndef ADDTASK_H
#define ADDTASK_H

#include <QDialog>
#include <QMap>
namespace Ui {
    class AddTask;
}

class AddTask : public QDialog
{
    Q_OBJECT

public:
    explicit AddTask(QWidget *parent = 0);
    ~AddTask();
    QMap<QString, QString> map;
private slots:
    void on_toolButton_clicked();

    void on_buttonBox_accepted();

private:
    Ui::AddTask *ui;
};

#endif // ADDTASK_H
