#include "headers/taskdb.h"

TaskDB::TaskDB() :
    QObject()
{

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
   // return true;
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
