#ifndef QPARSESITE_H
#define QPARSESITE_H

#include <QSharedData>
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
    QParseSite(QObject *parent = 0);
    QNetworkAccessManager *nam;
    int parent_page;
    bool parsed;
    int total;
    QUrl url_current;
    QStringList links;
    QStringList media;
    void run();
    QUrl siteurl;
private:

private slots:
    void get_page(QUrl url);
    void parse_page(QString content);
signals:
    void page_parsed(QStringList slinks,QStringList smedia,QString content,QString msg);

public slots:

   void parseSite(QString url);

   void finishedSlot(QNetworkReply*);

};

#endif // QPARSESITE_H
