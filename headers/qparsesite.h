#ifndef QPARSESITE_H
#define QPARSESITE_H

#include <QSharedData>
#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include "headers/taskdb.h"
class QParseSite : public TaskDB
{
    Q_OBJECT
public:
    explicit QParseSite(QObject *parent = 0,QString db = "");
    QNetworkAccessManager* nam;
    ///TaskDB* taskdb;
    int parent_page;
    bool parsed;
public slots:
    void handleLogMessage(QString msg);
private:
    QString siteurl;

signals:
    void log(QString msg);
private slots:
    void get_page(QUrl url);
    void parse_page(QString content);


public slots:
   void finishedSlot(QNetworkReply*);
   void parseSite(QString url);


};

#endif // QPARSESITE_H
