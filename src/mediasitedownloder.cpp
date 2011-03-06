#include "headers/mediasitedownloder.h"
#include "ui_mediasitedownloder.h"
#define taskdir QDir::toNativeSeparators ( QApplication::applicationDirPath()+QDir::toNativeSeparators("/tasks/"))

MediaSiteDownloder::MediaSiteDownloder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaSiteDownloder)
{
    ui->setupUi(this);
    parsing = false;
    ///taskdb=new TaskDB();
    QMenuBar* bar=this->menuBar();
    QList<QAction *> actions = bar->actions();
    QList<QAction *>::const_iterator it = actions.begin();
    for(; it != actions.end(); it++)
    {
        QAction *action = *it;
        if(action->menu()->objectName()=="menuOptions")
        {
            createLanguageMenu();
            action->menu()->addMenu(languageMenu);
        }
    }
    taskdb.setFolder(taskdir);
    init_app();
}
void MediaSiteDownloder::init_app()
{

    dir.setPath(taskdir);
    QStringList fileNames =
            dir.entryList(QStringList("*.project"));
    ui->tasklist->clear();
    foreach (QString name, fileNames) {
        name.replace(".project","");
        ui->tasklist->addItem(name);
    }

}

MediaSiteDownloder::~MediaSiteDownloder()
{
    delete ui;
}

void MediaSiteDownloder::on_actionExit_triggered()
{
    if(parsing)
    {
        QMessageBox msgBox;
        msgBox.setText(tr("Application is working now."));
        msgBox.setInformativeText(tr("Do you realy want to close application?"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            taskdb.close();
            exit(0);
        }
    }
    else
    {
      exit(0);
    }

}

void MediaSiteDownloder::on_actionAbout_QT_triggered()
{
    QApplication::aboutQt();
}

void MediaSiteDownloder::createLanguageMenu()
{
    languageMenu = new QMenu(tr("langmenu"),this);
    languageMenu->setIcon(QIcon(":/res/config-language.png"));
    QActionGroup *languageActionGroup = new QActionGroup(this);
    connect(languageActionGroup, SIGNAL(triggered(QAction *)),
            this, SLOT(switchLanguage(QAction *)));

    QDir dir(QDir::toNativeSeparators (QApplication::applicationDirPath()+"/lang/" ));
    QStringList fileNames =
            dir.entryList(QStringList("qmsd_*.qm"));
    QString syslocale = QLocale::system().name();
    QString lang;
    if(syslocale.length()>2)
    {
        syslocale.resize(2);
    }
    for (int i = 0; i < fileNames.size(); ++i) {
        QString locale = fileNames[i];
        locale.remove(0, locale.indexOf('_') + 1);
        locale.truncate(locale.lastIndexOf('.'));
        QTranslator translator1;
        translator1.load(QDir::toNativeSeparators (QApplication::applicationDirPath()+"/lang/" )+fileNames[i]);
        QString language = translator1.translate("Language","English");
        QAction *action = new QAction(tr("&%2").arg(language), this);
        action->setCheckable(true);
        action->setData(locale);
        languageMenu->addAction(action);
        languageActionGroup->addAction(action);
        if(settings.value("locale","none")=="none")
        {
            if (locale== syslocale)
            {
                action->setChecked(true);
                lang=locale;
            }
        }
        else
        {
            if (locale==settings.value("locale","none"))
            {
                action->setChecked(true);
                lang=settings.value("locale","none").toString();
            }
        }

    }
    translator.load(QDir::toNativeSeparators (QApplication::applicationDirPath()+"/lang/" )+"qmsd_"+lang);
    QApplication::installTranslator(&translator);
    languageMenu->setTitle(tr("langmenu"));
}

void MediaSiteDownloder::switchLanguage(QAction *action)
{
    QString locale = action->data().toString();
    if(settings.value("langtodef",false)==true)
        settings.setValue("locale",locale);
    else
        settings.remove("locale");
    translator.load("qmsd_" + locale, QDir::toNativeSeparators (QApplication::applicationDirPath()+"/lang/"));
    QApplication::installTranslator(&translator);
    languageMenu->setTitle(tr("langmenu"));
}
void MediaSiteDownloder::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}


void MediaSiteDownloder::on_tasklist_customContextMenuRequested(QPoint pos)
{
    if(ui->tasklist->currentIndex().isValid() && ui->tasklist->currentItem()->isSelected()){
        QMenu *m=new QMenu();
        pos.setX(pos.x()-5);
        pos.setY(pos.y()+25);
        m->addAction(ui->actionRemove_Task);
        //m->addAction(ui->actionEdit);
        m->exec(ui->tasklist->mapToGlobal(pos));
    }
}

#include "headers/addtask.h"
#include <QStandardItem>
void MediaSiteDownloder::on_actionNew_Task_triggered()
{
    AddTask *task = new AddTask(this);
    if(task->exec()==QDialog::Accepted)
    {
        QString dbname=QCryptographicHash::hash (task->map.value("taskname").toAscii(), QCryptographicHash::Md5 ).toHex();
        taskdb.createTask(dbname);
        QSettings taskset(taskdir+task->map.value("taskname")+".project",QSettings::IniFormat);
        taskset.setValue("dbname",dbname);
        foreach (QString str, task->map.keys())
            taskset.setValue(str,task->map.value(str));
        taskset.setValue("creation_date",QDateTime::currentDateTime ().toString());
        taskdb.close();
        taskset.sync();
        init_app();
    }

}

void MediaSiteDownloder::on_tasklist_itemDoubleClicked(QListWidgetItem* item)
{
    if(!item->text().isEmpty())
    {
        QSettings tasksettings(taskdir+item->text()+".project",QSettings::IniFormat);
        ui->task_url->setText(tasksettings.value("url","null").toString());
        taskdb.open(tasksettings.value("dbname","null").toString());
        media_path = tasksettings.value("media_path","null").toString();
        ui->date_added->setText(tasksettings.value("creation_date","null").toString());
        ui->pages_crawled->setText(QString::number(taskdb.count_crawld()));
        ui->mediadowned->setText(QString::number(taskdb.count_media_downed()));

        if(taskdb.count_media()>0)
                update_media_list();
        ui->main_info->setEnabled(true);

    }
}
void MediaSiteDownloder::on_startscan_clicked()
{

    ui->actionTask_List->setEnabled(true);
    ui->index->hide();
    ui->parse_info->show();
    ui->tasktabs->setCurrentIndex(0);
    page_index=taskdb.add_page(ui->task_url->text());
    ui->curent_cheking->setText(ui->task_url->text());
    time.start();
    updateDisplay();
    connect(&timer, SIGNAL(timeout()), this, SLOT(updateDisplay()));
    timer.start(1000);
    site.start();
    ui->mainToolBar->setEnabled(false);
    connect(&site, SIGNAL(page_parsed(QStringList,QStringList,QString,QString)),this,SLOT(save_page_parsed(QStringList,QStringList,QString,QString)));
    parsing =  true;
    site.siteurl.setUrl(ui->task_url->text(),QUrl::TolerantMode);
    site.parseSite(ui->task_url->text());
    ///  site.run();
}


void MediaSiteDownloder::save_page_parsed(QStringList links, QStringList media,QString content,QString msg)
{
    parsing = false;
    ui->curent_cheking->setText(tr(" Saving ...")+ui->curent_cheking->text());
    if(msg.isEmpty())
    {
        taskdb.set_page_parsed(page_index,1,"","");//content
        if(media.count()>0)
        {
            taskdb.add_media(media,page_index);
            update_media_list();
            ui->media_num->setText(QString::number(taskdb.count_media()));
        }
        taskdb.add_page(links);
        ui->numder_checked->setText(QString::number(taskdb.count_crawld()));
        ui->left_num->setText(QString::number(taskdb.count_left()));
    }
    else{
        taskdb.set_page_parsed((int)page_index, 2,"" ,msg);
        ui->log->addItem(msg);
    }
    QString page = taskdb.get_next_page();
    if(!page.isEmpty())
    {
        parsing = true;
        page_index = taskdb.page_index;
        site.parseSite(page);
        ui->curent_cheking->setText(page);
    }

}

void MediaSiteDownloder::update_media_list()
{
    media_map.clear();
    media_map=taskdb.media_files();
    QMapIterator<QString,QMap<QString,QString> > i(media_map);
    while (i.hasNext()) {
         i.next();
         QListWidgetItem* item = new QListWidgetItem();//(i.key());
item->setText(i.key());
        /// item.setData(Qt::UserRole,m);
         item->setSelected(true);
         ui->medialist->insertItem(0,i.key());
         qDebug() << i.key() << ": " << i.value() << endl;
     }
}

void MediaSiteDownloder::updateDisplay()
{
    int secs = time.elapsed() / 1000;
    int mins = (secs / 60) % 60;
    int hours = (secs / 3600);
    secs = secs % 60;    void add_page(QStringList pages);
    ui->total_time->setText(QString("%1:%2:%3")
                            .arg(hours, 2, 10, QLatin1Char('0'))
                            .arg(mins, 2, 10, QLatin1Char('0'))
                            .arg(secs, 2, 10, QLatin1Char('0')) );

}
void MediaSiteDownloder::handleLogMessage(QString msg)
{
    ui->log->addItem(QDateTime::currentDateTime ().toString()+"  "+msg);
}

#include "headers/errorlogui.h"
void MediaSiteDownloder::on_pushButton_clicked()
{
    ErrorLogUi* logui = new ErrorLogUi(this);
    logui->build_list(taskdb.pages_with_error());
    logui->exec();
    delete logui;
}

void MediaSiteDownloder::on_viewMediaList_clicked()
{
    ui->actionTask_List->setEnabled(true);
    ui->index->hide();
    ui->parse_info->show();
    ui->tasktabs->setCurrentIndex(1);
}

void MediaSiteDownloder::on_actionTask_List_triggered()
{
        ui->actionTask_List->setEnabled(false);

        ui->parse_info->hide();
        parsing = false;
        ui->index->show();
}
