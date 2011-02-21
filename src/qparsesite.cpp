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

          QRegExp rx("<a[^>]+href=\"([^\"]+)\"[^>]*>(.*)</a>");



           rx.setMinimal(true);

           QStringList list;
           int pos = 0;
           while((pos = rx.indexIn(content, pos)) != -1)
           {
               list << rx.cap(1);
               pos += rx.matchedLength();

           }
           list.removeDuplicates();

          // qthisebug()<<;
           this->add_media(list.filter(".mp3"),parent_page);


          QStringList links=list.replaceInStrings(QRegExp(".*.mp3"),"_");
          links.removeDuplicates();
          QUrl link;
          int iternal ;
           for( int n = 0; n < links.size(); n++)
           {
               link.setUrl(links.at( n));
               iternal =links.at( n).indexOf("/");
               qDebug()<<iternal;
                qDebug()<<links.at(n);
               if(siteurl != link.host() && iternal!=0)
                   links.removeAt(n);
               if(iternal==0)
               {
               links.replace(n,"http://"+siteurl+links.at(n));
               ///  links.removeAt(n);
               }
            //qthisebug()<<szTmp;

           }

           foreach (QString site_one, links) {
               qDebug()<<site_one;
               parseSite(site_one);

           }


     ///<a[^>]+href="([^"]+)"[^>]*>(.*?)</a>
    }


}

void QParseSite::parseSite(QString url = "")
{
    emit dblog("Startethis parsing "+url);
   if(!this->page_exists(url))
            get_page(QUrl(url));
}

void QParseSite::get_page(QUrl url)
{
   if(url.scheme()=="http" || url.scheme()=="https" || url.scheme()=="ftp")
    {
        siteurl = url.host();
        parent_page=this->add_page(url.toString());
        nam->get(QNetworkRequest(url));
    }

}
