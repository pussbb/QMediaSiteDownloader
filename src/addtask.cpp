#include "headers/addtask.h"
#include "ui_addtask.h"
#include "QUrl"
#include "QClipboard"
#include <QFileDialog>
#include <QMessageBox>

AddTask::AddTask(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddTask)
{
    ui->setupUi(this);
    QClipboard* clipboard;
    QUrl *uri= new QUrl(clipboard->text(QClipboard::Clipboard),QUrl::TolerantMode);
        if(uri->scheme()!="http" && uri->scheme()!="https" && uri->scheme()!="ftp")
        {  uri->setUrl("http://");}
    ui->taskurl->setText(uri->toString());
}

AddTask::~AddTask()
{
    delete ui;
}

void AddTask::on_toolButton_clicked()
{
    ui->pathtomedia->setText(QFileDialog::getExistingDirectory(this, tr("Choose folder to save"),
                                                               QDir::currentPath()));
}
