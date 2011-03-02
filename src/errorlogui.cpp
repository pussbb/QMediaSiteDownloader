#include "headers/errorlogui.h"
#include "ui_errorlogui.h"

ErrorLogUi::ErrorLogUi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ErrorLogUi)
{
    ui->setupUi(this);
}

ErrorLogUi::~ErrorLogUi()
{
    delete ui;
}
void ErrorLogUi::build_list(QStringList list)
{
    ui->errrolist->clear();
    if(list.count() > 0)
    {
        ui->errrolist->addItems(list);
    }
    else
    {
        ui->errrolist->addItem("There is no errors.");
    }
}

void ErrorLogUi::on_closeuilog_clicked()
{
    this->close();
}
