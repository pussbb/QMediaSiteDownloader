#include "headers/mediasitedownloder.h"
#include "ui_mediasitedownloder.h"
#define taskdir QDir::toNativeSeparators ( QApplication::applicationDirPath()+"/tasks/" )

MediaSiteDownloder::MediaSiteDownloder(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MediaSiteDownloder)
{
    ui->setupUi(this);
    taskdb = new TaskDB;
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
    ///connect(taskdb,SIGNAL(dblog(QString)),this,SLOT(handleLogMessage(QString)));
    taskdb->setFolder(taskdir);
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
    exit(0);
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
    if(task->exec()== QDialog::Accepted)
    {
       QString dbname=QCryptographicHash::hash (task->map.value("taskname").toAscii(), QCryptographicHash::Md5 ).toHex();
       taskdb->createTask(dbname);
       QSettings taskset(taskdir+task->map.value("taskname")+".project",QSettings::NativeFormat);
       taskset.setValue("dbname",dbname);
       foreach (QString str, task->map.keys())
           taskset.setValue(str,task->map.value(str));
       taskdb->close();
       init_app();
    }

}

void MediaSiteDownloder::on_tasklist_itemDoubleClicked(QListWidgetItem* item)
{
    if(!item->text().isEmpty())
    {
           QSettings tasksettings(taskdir+item->text()+".project",QSettings::NativeFormat);
           qDebug()<<tasksettings.fileName();
            ui->task_url->setText(tasksettings.value("url","null").toString());
    }
}
#include "headers/qparsesite.h"
void MediaSiteDownloder::on_startscan_clicked()
{
    ui->index->hide();
    ui->parse_info->show();
   /// ui->log->clear();
    QParseSite* site = new QParseSite(this,"b07e93a3f4c43389c8524c00d45900db");
    connect(site,SIGNAL(dblog(QString)),this,SLOT(handleLogMessage(QString)));
    site->parseSite("http://mfm.ua/top2010/");

}
void MediaSiteDownloder::handleLogMessage(QString msg)
{

    qDebug()<<taskdb->db.databaseName();
    ui->log->addItem(QDateTime::currentDateTime ().toString()+"  "+msg);
}
