#include "headers/medisitedownloder.h"
#include "ui_medisitedownloder.h"

MediSiteDownloder::MediSiteDownloder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediSiteDownloder)
{
    ui->setupUi(this);
}

MediSiteDownloder::~MediSiteDownloder()
{
    delete ui;
}
