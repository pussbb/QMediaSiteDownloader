#ifndef MEDIASITEDOWNLODER_H
#define MEDIASITEDOWNLODER_H

#include <QMainWindow>
#include <QDir>
#include <QTranslator>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include "headers/taskdb.h"
///
#include <QDebug>
///

namespace Ui {
    class MediaSiteDownloder;
}

class MediaSiteDownloder : public QMainWindow
{
    Q_OBJECT

public:
    explicit MediaSiteDownloder(QWidget *parent = 0);
    ~MediaSiteDownloder();
    QDir dir;
    TaskDB taskdb;
    QTranslator translator;
    QMessageBox msgBox;
    QSettings settings;
    QMenu *languageMenu;

protected:
    void changeEvent(QEvent *e);

private slots:
    void init_app();
    void on_actionExit_triggered();
    void switchLanguage(QAction *action);
    void createLanguageMenu();
    void on_actionAbout_QT_triggered();


    void on_tasklist_customContextMenuRequested(QPoint pos);

    void on_actionNew_Task_triggered();

private:
    Ui::MediaSiteDownloder *ui;
};

#endif // MEDISITEDOWNLODER_H
