#include "headers/taskdb.h"
#include "QApplication"
TaskDB::TaskDB() :
    QObject()
{
        taskfolder=QDir::toNativeSeparators ( QApplication::applicationDirPath()+"/tasks/" ) ;
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
    return true;
}
bool TaskDB::page_exists(QString page)
{
    QSqlQuery sql("SELECT *  FROM pages WHERE url =\""+page+"\" ORDER BY rowid; ");
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

     if(sql.lastError().isValid())
          qDebug()<<sql.lastError().text();

     return sql.lastInsertId().toInt();

}

void TaskDB::add_media(QStringList list, int parent)
{
    QSqlQuery sql;
    QSqlQuery exists;///();
    for (int i = 0; i < list.size(); ++i)
    {
        exists.exec("SELECT *  FROM media WHERE url =\""+list.at(i)+"\" ORDER BY id; ");
        if(!exists.next())
        {
               qDebug()<<sql.lastError().text(); sql.exec("INSERT INTO media(from_page,url,downloaded) VALUES("+QString::number(parent)+",\""+list.at(i)+"\",0);");
            if(sql.lastError().isValid())
            {}
            else{

            }
         }
        else{

        }
    }
}

void TaskDB::set_page_parsed(int id)
{
    QSqlQuery sql;
    sql.exec("UPDATE pages SET parsed=1 WHERE id="+QString::number(id)+";");
    qDebug()<<sql.lastError().text();
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
