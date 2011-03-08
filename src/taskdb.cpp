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

void TaskDB::set_page_parsed(int id,int val, QString error)
{
    QSqlQuery sql;
     error.replace("'","\"\"");
    sql.exec("UPDATE pages SET parsed="+QString::number(val)+", error_str='"+error+"' WHERE id="+QString::number(id)+";");
}
void TaskDB::set_page_parsed(int id,int val,QString content, QString error)
{
    QSqlQuery sql;
    content.replace("'","\"\"");
    error.replace("'","\"\"");
    sql.exec("UPDATE pages SET parsed="+QString::number(val)+",content='"+content+"', error_str='"+error+"' WHERE id="+QString::number(id)+";");
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
int TaskDB::count_media_downed()
{
    QSqlQuery sql;
    sql.exec("select count(*) from media where downloaded = 1");
    if(sql.next())
        return sql.value(0).toInt();
    return -1;
}
int TaskDB::count_media()
{
    QSqlQuery sql;
    sql.exec("select count(*) from media");
    if(sql.next())
        return sql.value(0).toInt();
    return -1;
}
QMap<QString,QMap<QString,QString> > TaskDB::media_files()
{
    QMap<QString,QMap<QString,QString> > media;
    QSqlQuery sql;
    sql.exec("SELECT pages.id, pages.url , media.id ,media.url,media.downloaded FROM media INNER JOIN pages ON media.from_page = pages.id");
    QFileInfo file;
    while(sql.next())
    {
        QMap<QString,QString> details;
        details.insert("page_id",sql.value(0).toString());
        details.insert("page_url",sql.value(1).toString());
        details.insert("id",sql.value(2).toString());
        details.insert("url",sql.value(3).toString());
        details.insert("downed",sql.value(4).toString());
        file.setFile(sql.value(3).toString());
        media.insert(file.fileName(),details);
    }
    return media;
}

QStringList TaskDB::pages_with_error()
{
    QSqlQuery sql;
    sql.exec("SELECT error_str FROM pages WHERE parsed = 2");
    QStringList list;
    if(sql.lastError().isValid())
        return list<<tr("Error occurred while fetching results?");
    while(sql.next())
    {
        list<<sql.value(0).toString();
    }
    return list;
}
QStringList TaskDB::log_error()
{
    QSqlQuery sql;
    sql.exec("SELECT error_str FROM pages WHERE parsed = 2");
    QStringList list;
    if(sql.lastError().isValid())
        list<<tr("Error occurred while fetching results?");
    while(sql.next())
    {
        list<<sql.value(0).toString();
    }
    sql.exec("SELECT error FROM media WHERE downloaded =2 ");
    while(sql.next())
    {
        list<<sql.value(0).toString();
    }
    //list.removeDuplicates();
    return list;
}
void TaskDB::set_media_down(QString error, int id, int val)
{
    QSqlQuery sql;
    error.replace("'","\"\"");
    sql.exec("UPDATE media SET downloaded="+QString::number(val)+", error='"+error+"' WHERE id="+QString::number(id)+";");

}
