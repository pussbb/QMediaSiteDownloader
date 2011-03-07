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
    stop = false;
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
        pos.setY(pos.y()+5);
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
    ui->actionRefresh_Media_List->setEnabled(true);
    ui->actionMedia_down->setEnabled(true);
    ui->actionMedia_Up->setEnabled(true);
    ui->actionShow_Errors->setEnabled(true);
    ui->actionTask_List->setEnabled(true);
    ui->actionNew_Task->setEnabled(false);
    ui->actionRemove_Task->setEnabled(false);
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
    connect(&site, SIGNAL(page_parsed(QStringList,QStringList,QString,QString)),this,SLOT(save_page_parsed(QStringList,QStringList,QString,QString)));
    parsing =  true;
    site.siteurl.setUrl(ui->task_url->text(),QUrl::TolerantMode);
    site.parseSite(ui->task_url->text());
    ///  site.run();
}


void MediaSiteDownloder::save_page_parsed(QStringList links, QStringList media,QString content,QString msg)
{
    parsing = false;
    Q_UNUSED(content);
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
    if(!stop && !ui->scanurl->isChecked())
    {
        QString page = taskdb.get_next_page();
        if(!page.isEmpty())
        {
            parsing = true;
            page_index = taskdb.page_index;
            site.parseSite(page);
            ui->curent_cheking->setText(page);
        }
    }
    else
    {
        timer.stop();
        msgBox.setText("Given url was successfuly parsed.");
        msgBox.exec();
    }

}

void MediaSiteDownloder::update_media_list()
{
    media_map.clear();
    ui->medialist->clear();
    media_map=taskdb.media_files();
    QMapIterator<QString,QMap<QString,QString> > i(media_map);
    QString text;
    while (i.hasNext()) {
        i.next();
        QListWidgetItem* item = new QListWidgetItem();//(i.key());
        text =  i.key().toLocal8Bit();
        item->setText(text);
        item->setData(Qt::UserRole,i.key());

        if(i.value()["downed"].toInt() == 0)
        {
            item->setIcon(QIcon(":/toolbar/Symbol-Error.png"));
            item->setCheckState(Qt::Checked);
        }
        else
        {
            item->setCheckState(Qt::Unchecked);
            item->setIcon(QIcon(":/toolbar/Symbol-Add.png"));
        }
        item->setSelected(true);
        ui->medialist->insertItem(0,item);
        ///qDebug() << i.key() << ": " << i.value() << endl;
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
    ui->actionRefresh_Media_List->setEnabled(true);
    ui->actionMedia_down->setEnabled(true);
    ui->actionMedia_Up->setEnabled(true);
    ui->actionShow_Errors->setEnabled(true);
    ui->actionNew_Task->setEnabled(false);
    ui->actionRemove_Task->setEnabled(false);
    ui->index->hide();
    ui->parse_info->show();
    ui->tasktabs->setCurrentIndex(1);
}

void MediaSiteDownloder::on_actionTask_List_triggered()
{
    ui->actionTask_List->setEnabled(false);
    ui->actionRefresh_Media_List->setEnabled(false);
    ui->actionMedia_down->setEnabled(false);
    ui->actionMedia_Up->setEnabled(false);
    ui->actionShow_Errors->setEnabled(false);
    ui->actionNew_Task->setEnabled(true);
    ui->actionRemove_Task->setEnabled(true);
    stop = true;
    ui->parse_info->hide();
    parsing = false;
    ui->index->show();
}

void MediaSiteDownloder::on_medialist_customContextMenuRequested(QPoint pos)
{
    if(ui->medialist->currentIndex().isValid() && ui->medialist->currentItem()->isSelected()){
        QMenu *m=new QMenu();
        pos.setX(pos.x()-5);
        pos.setY(pos.y()+5);
        m->addAction(ui->actionGo_to_Page);
        m->addAction(ui->actionCopy_to_Clipboard);
        m->exec(ui->medialist->mapToGlobal(pos));
    }
}

void MediaSiteDownloder::on_actionMedia_Up_triggered()
{
    if(ui->medialist->currentIndex().isValid() && ui->medialist->currentItem()->isSelected()){

        QListWidgetItem *current = ui->medialist->currentItem();
        int currIndex = ui->medialist->row(current);

        QListWidgetItem *prev =ui->medialist->item(ui->medialist->row(current) - 1);
        int prevIndex = ui->medialist->row(prev);

        QListWidgetItem *temp = ui->medialist->takeItem(prevIndex);
        ui->medialist->insertItem(prevIndex, current);
        ui->medialist->insertItem(currIndex, temp);
    }
}

void MediaSiteDownloder::on_actionShow_Errors_triggered()
{
    ErrorLogUi* logui = new ErrorLogUi(this);
    logui->build_list(taskdb.pages_with_error());
    logui->exec();
    delete logui;
}


void MediaSiteDownloder::on_actionMedia_down_triggered()
{
    QListWidgetItem *current = ui->medialist->currentItem();
    int currIndex = ui->medialist->row(current);
    QListWidgetItem *next = ui->medialist->item(ui->medialist->row(current) + 1);
    int nextIndex = ui->medialist->row(next);
    QListWidgetItem *temp = ui->medialist->takeItem(nextIndex);

    ui->medialist->insertItem(currIndex, temp);
    ui->medialist->insertItem(nextIndex, current);
}

void MediaSiteDownloder::on_toolmediaup_clicked()
{
    this->on_actionMedia_Up_triggered();
}

void MediaSiteDownloder::on_toolmediadown_clicked()
{
    this->on_actionMedia_down_triggered();
}

void MediaSiteDownloder::on_actionRefresh_Media_List_triggered()
{
    update_media_list();
}
#include <QDesktopServices>
void MediaSiteDownloder::on_actionGo_to_Page_triggered()
{
    if(ui->medialist->currentIndex().isValid() && ui->medialist->currentItem()->isSelected()){
    QDesktopServices::openUrl(QUrl(media_map[ui->medialist->currentItem()->data(Qt::UserRole).toString()]["page_url"], QUrl::TolerantMode));
    }

}
#include "QClipboard"
void MediaSiteDownloder::on_actionCopy_to_Clipboard_triggered()
{

        if(ui->medialist->currentIndex().isValid() && ui->medialist->currentItem()->isSelected()){
            QClipboard *clipboard = QApplication::clipboard();
        clipboard->setText(media_map[ui->medialist->currentItem()->data(Qt::UserRole).toString()]["url"]);
        }

}

void MediaSiteDownloder::on_actionRemove_Task_triggered()
{
    if(ui->tasklist->currentIndex().isValid() && ui->tasklist->currentItem()->isSelected()){
        QMessageBox msgBox;
        msgBox.setText(tr("Do you realy want to delete task"));
        msgBox.setStandardButtons(QMessageBox::Yes | QMessageBox::Cancel);
        msgBox.setDefaultButton(QMessageBox::Cancel);
        int ret = msgBox.exec();
        if(ret == QMessageBox::Yes)
        {
            QSettings set(taskdir+ui->tasklist->currentItem()->text()+".project",QSettings::IniFormat);
            QFile file;
            QString dbname = taskdir+set.value("dbname","").toString()+".task";
            qDebug()<<dbname;
            QString current_dbname = taskdb.db.databaseName();
            if(dbname == current_dbname )
                 taskdb.close();
            file.setFileName(dbname);
            file.remove();
            file.setFileName(taskdir+ui->tasklist->currentItem()->text()+".project");
            file.remove();
            ui->tasklist->removeItemWidget(ui->tasklist->currentItem());
            init_app();
        }

    }
}
