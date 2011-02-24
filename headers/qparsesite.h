#ifndef QPARSESITE_H
#define QPARSESITE_H

#include <QSharedData>
#include <QObject>
#include <QThread>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QDebug>
#include "headers/taskdb.h"
#include <QRegExp>
#include <QStringList>
class QParseSite :  public QThread
{
    Q_OBJECT
public:
    explicit QParseSite();
    QNetworkAccessManager* nam;
    int parent_page;
    bool parsed;
    int total;
    QStringList links;
    QStringList media;
private:
    QString siteurl;
private slots:
    void get_page(QUrl url);
    void parse_page(QString content);
signals:
    void page_parsed(QStringList slinks,QStringList smedia,QString msg);

public slots:
   void finishedSlot(QNetworkReply*);
   void parseSite(QString url);


};

#endif // QPARSESITE_H
