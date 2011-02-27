#ifndef MEDIASITEDOWNLODER_H
#define MEDIASITEDOWNLODER_H

#include <QMainWindow>
#include <QDir>
#include <QTranslator>
#include <QMenu>
#include <QInputDialog>
#include <QMessageBox>
#include <QSettings>
#include <QListWidgetItem>
#include "headers/taskdb.h"
#include "headers/qparsesite.h"
#include <QTime>
#include <QTimer>
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
    QParseSite site;
    QMenu *languageMenu;
    int page_index;
    QString media_path;
    QTime time;
    QTimer timer;
protected:
    void changeEvent(QEvent *e);
public slots:
    void handleLogMessage(QString msg);
    void save_page_parsed(QStringList links,QStringList media,QString msg);
private slots:

    void init_app();
    void on_actionExit_triggered();
    void switchLanguage(QAction *action);
    void createLanguageMenu();
    void on_actionAbout_QT_triggered();
    void on_tasklist_customContextMenuRequested(QPoint pos);
    void on_actionNew_Task_triggered();
    void on_tasklist_itemDoubleClicked(QListWidgetItem* item);
    void on_startscan_clicked();
    void updateDisplay();

private:
    bool parsing;
    Ui::MediaSiteDownloder *ui;
};

#endif // MEDISITEDOWNLODER_H
