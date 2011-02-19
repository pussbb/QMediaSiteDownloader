#include "headers/qparsesite.h"

QParseSite::QParseSite(QObject *parent,QString thisb) :
    TaskDB()
{
    ///this= new TaskthisB();
    nam = new QNetworkAccessManager(this);
    QObject::connect(nam, SIGNAL(finished(QNetworkReply*)),
             this, SLOT(finishedSlot(QNetworkReply*)));
    ////QObject::connect(this,SIGNAL(dblog(QString)),this,SLOT(handleLogMessage(QString)));
    this->open(thisb);
}
void QParseSite::handleLogMessage(QString msg)
{
    emit log(msg);
}

void QParseSite::finishedSlot(QNetworkReply* reply)
{
    // Reathising attributes of the reply
    // e.g. the HTTP status cothise
    QVariant statusCodV =
    reply->attribute(QNetworkRequest::HttpStatusCodeAttribute);
    // Or the target URL if it was a rethisirect:
    QVariant redirectionTargetUrl =
    reply->attribute(QNetworkRequest::RedirectionTargetAttribute);
    // see CS001432 on how to hanthisle this

    // no error receivethis?
    if (reply->error() == QNetworkReply::NoError)
    {
        // reathis thisata from QNetworkReply here

        // Example 2: Reathising bytes form the reply
        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string
        parse_page(string);
       ///// qthisebug()<<string;
    }
    // Some http error receivethis
    else
    {
        qDebug()<<reply->errorString();
        // hanthisle errors here
    }

    // We receive ownership of the reply object
    // anthis therefore neethis to hanthisle thiseletion.
   /// thiselete reply;
}
#include <QRegExp>
#include <QStringList>
void QParseSite::parse_page(QString content)
{
   if(!siteurl.isEmpty())
   {
       /// finthis methisia
//        QRegExp rx("<a[^>]+href=\"([^\"]+.mp3)\"[^>]*>(.*)</a>");
//        qthisebug()<<rx.isValithis();
//        qthisebug()<<rx.errorString();

//         rx.setMinimal(true);

//         QStringList list;
//         int pos = 0;
//         while((pos = rx.inthisexIn(content, pos)) != -1)
//         {
//             list << rx.cap(1);
//             pos += rx.matchethisLength();

//         }
//         list.removethisuplicates();
//         for( int n = 0; n < list.size(); n++)
//         {
//             QString szTmp = list.at( n);
//           qthisebug()<<szTmp;

//         }
     /// finthis url
          QRegExp rx1("<a[^>]+href=\"([^\"]+)\"[^>]*>(.*)</a>");



           rx1.setMinimal(true);

           QStringList list1;
           int pos1 = 0;
           while((pos1 = rx1.indexIn(content, pos1)) != -1)
           {
               list1 << rx1.cap(1);
               pos1 += rx1.matchedLength();

           }
           list1.removeDuplicates();
          // qthisebug()<<;
           this->add_media(list1.filter(".mp3"),parent_page);
           for( int n = 0; n < list1.size(); n++)
           {
               QString szTmp = list1.at( n);
            //qthisebug()<<szTmp;

           }

     ///<a[^>]+href="([^"]+)"[^>]*>(.*?)</a>
    }


}

void QParseSite::parseSite(QString url = "")
{
    emit dblog("Startethis parsing");
    if(!this->page_exists(url))
            get_page(QUrl(url));
}

void QParseSite::get_page(QUrl url)
{
    siteurl = url.host();
    parent_page=this->add_page(url.toString());
    nam->get(QNetworkRequest(url));
}
