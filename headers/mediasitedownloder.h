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
#include <QMap>
#include <QDateTime>
#include "headers/downloadfile.h"
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
    QString current_task;
    bool download_stoped;
    bool is_downloading;
    int current_media_index;
    QString current_media_text;
    QTimer timer;
    QMap<QString,QMap<QString,QString> > media_map;
    DownloadFile *download;
protected:
    void changeEvent(QEvent *e);
public slots:
    void handleLogMessage(QString msg);
    void save_page_parsed(QStringList links,QStringList media,QString content,QString msg);
    void Download_media_Finished(QString error, bool finished);
private slots:
    void update_media_list();
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
    void download_all();

    void on_pushButton_clicked();
    void on_viewMediaList_clicked();
    void on_actionTask_List_triggered();
    void on_medialist_customContextMenuRequested(QPoint pos);
    void on_actionMedia_Up_triggered();
    void on_actionShow_Errors_triggered();
    void on_actionMedia_down_triggered();
    void on_toolmediaup_clicked();
    void on_toolmediadown_clicked();
    void on_actionRefresh_Media_List_triggered();
    void on_actionGo_to_Page_triggered();
    void on_actionCopy_to_Clipboard_triggered();
    void on_actionRemove_Task_triggered();
    void on_start_download_clicked();
    void on_stop_download_clicked();

    void on_toolButton_zoomin_clicked();

    void on_toolButton_zoomout_clicked();

    void on_save_task_settings_clicked();

    void on_actionOpen_Directory_triggered();

    void on_actionPlay_triggered();

    void on_actionAbout_triggered();

    void on_repeatscan_clicked();

private:
    bool stop;
    bool parsing;
    Ui::MediaSiteDownloder *ui;
};

#endif // MEDISITEDOWNLODER_H
