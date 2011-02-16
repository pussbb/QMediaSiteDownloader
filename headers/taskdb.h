#ifndef TASKDB_H
#define TASKDB_H

#include <QObject>
#include <QDir>
#include <QMessageBox>
#include <QtSql>

class TaskDB : public QObject
{
    Q_OBJECT
public:
    explicit TaskDB();

    QSqlDatabase db;
    void setFolder(QString foldername);
    bool createTask(QString name);
    inline void close(){db.close();}
signals:

private:
    QString taskfolder;
public slots:

};

#endif // TASKDB_H
