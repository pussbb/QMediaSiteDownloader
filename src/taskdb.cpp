#include "headers/taskdb.h"
#include "QApplication"
TaskDB::TaskDB() :
    QObject()
{
  taskfolder= QApplication::applicationDirPath()+QDir::toNativeSeparators("/tasks/");
}

void TaskDB::setFolder(QString foldername)
{
    QDir dir;
    dir.setPath(foldername);
    if(!dir.exists())
    {
        if(!dir.mkdir(foldername))
        {
            QMessageBox::warning(0, QObject::tr("Error"), QObject::tr("Error creating directory:<br />")+foldername);
            ///return false;
        }
        else
        {
            QFile::setPermissions(foldername,QFile::ReadOwner | QFile::WriteOwner |
                                  QFile::ExeOwner |  QFile::ReadUser | QFile::WriteUser | QFile::ExeUser | QFile::ReadGroup |
                                  QFile::WriteGroup | QFile::ExeGroup | QFile::ReadOther | QFile::WriteOther | QFile::ExeOther);
        }
    }
    this->taskfolder=foldername;
}

bool TaskDB::createTask(QString name)
{

    QFile sqlres(":/task.sql");
    if (!sqlres.open(QIODevice::ReadOnly))
    {
        QMessageBox::warning(0,  QObject::tr("Task creation error"),
                             QObject::tr("Cannot open file :\n%1.")
                             .arg(sqlres.errorString()));
        return false;
    }
    if(db.isOpen())
        db.close();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(taskfolder+name+".task");
    if (!db.open()) {
        QMessageBox::critical(0, tr("Cannot open database"),
                              tr("Cannot open database."), QMessageBox::Cancel);
        return false;
    }
    QString query=sqlres.readAll();
    QSqlQuery sql;
    QStringList querylist=query.split(";");
    for (int i = 0; i < querylist.size(); ++i)
        sql.exec(querylist.at(i));
    return true;
}
bool TaskDB::open(QString dbname)
{
    if(db.isOpen())
        db.close();
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName(taskfolder+dbname+".task");
    if (!db.open()) {
        QMessageBox::critical(0, tr("Cannot open database"),
                              tr("Cannot open database."), QMessageBox::Cancel);
        return false;
    }
    QSqlQuery sql;
    sql.exec("PRAGMA synchronous = OFF;");
    sql.exec("PRAGMA temp_store = MEMORY;");
    sql.exec(" PRAGMA quick_check; ");
    sql.exec("PRAGMA journal_mode =  MEMORY;");
    return true;
}
bool TaskDB::page_exists(QString page)
{
    QSqlQuery sql;
    sql.exec("SELECT *  FROM pages WHERE url =\""+page+"\" ORDER BY rowid; ");
    if(sql.next())
    {
        page_parsed=0;
        if(sql.value(2).toInt()!=0)
            page_parsed=sql.value(2).toInt();
        page_index=sql.value(2).toInt();
        return true;
    }
    return false;
}

int TaskDB::add_page(QString page)
{
    QSqlQuery sql;
    sql.exec("INSERT INTO pages(url ,parsed) VALUES(\""+page+"\",0);");

//    if(sql.lastError().isValid())
//        qDebug()<<sql.lastError().text();

    return sql.lastInsertId().toInt();

}

void TaskDB::add_page(QStringList pages)
{
    foreach (QString page, pages) {
        add_page(page);
    }

}
void TaskDB::add_media(QStringList list, int parent)
{
    QSqlQuery sql;
    QSqlQuery exists;
    for (int i = 0; i < list.size(); ++i)
    {
        sql.exec("INSERT INTO media(from_page,url,downloaded) VALUES("+QString::number(parent)+",\""+list.at(i)+"\",0);");
    }
}

void TaskDB::set_page_parsed(int id,int val)
{
    QSqlQuery sql;
    sql.exec("UPDATE pages SET parsed="+QString::number(val)+" WHERE id="+QString::number(id)+";");
   /// qDebug()<<sql.lastError().text();
}

QString TaskDB::get_next_page()
{
    QSqlQuery sql;
    sql.exec("SELECT *  FROM pages WHERE parsed = 0 ORDER BY id; ");
    if(sql.next())
    {
        page_index=sql.value(0).toInt();
        return sql.value(1).toString();
    }
    else
        return "";
}

int TaskDB::count_crawld()
{
    QSqlQuery sql;
    sql.exec("select count(*) from pages where parsed = 1");
    if(sql.next())
        return sql.value(0).toInt();
    return -1;
}
int TaskDB::count_left()
{
    QSqlQuery sql;
    sql.exec("select count(*) from pages where parsed = 0");
    if(sql.next())
        return sql.value(0).toInt();
    return -1;
}
int TaskDB::count_media()
{
    QSqlQuery sql;
    sql.exec("select count(*) from media where downloaded = 0");
    if(sql.next())
        return sql.value(0).toInt();
    return -1;
}
void TaskDB::media_files()
{
    QSqlQuery sql;
    sql.exec("select * from media");
    while(sql.next())
    {
          qDebug() <<"url" << ": " << sql.value(1) <<"down:"<<sql.value(3)<< endl;
    }


}
