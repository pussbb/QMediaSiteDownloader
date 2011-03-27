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
    inline void close(){db.close();};
    int add_page(QString page);
    void add_page(QStringList pages);
    void add_media(QStringList list,int parent);
    bool open(QString dbname);
    bool page_exists(QString page);
    int page_index;
    int page_parsed;
    int inline db_version(){return 00011;};
    QMap<QString,QMap<QString,QString> > media_files();
    QString get_next_page();
    void set_page_parsed(int id,int val = 1 ,QString error = "");
    void set_page_parsed(int id,int val = 1,QString content = "", QString error = "");
    QStringList pages_with_error();
    int count_crawld();
    int count_left();
    int count_media();
    int count_media_downed();
    void reset_scaned();
    void set_media_down(QString error = "", int id = 0,int val =1);
    QStringList  log_error();

signals:
    void dblog(QString msg);
private:
    QString taskfolder;
public slots:

};

#endif // TASKDB_H
