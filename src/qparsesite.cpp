#include "headers/qparsesite.h"

QParseSite::QParseSite(QObject *parent): QThread(parent)

{
    nam = new QNetworkAccessManager();
    connect(nam, SIGNAL(finished(QNetworkReply*)), SLOT(finishedSlot(QNetworkReply*)));
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
        QByteArray bytes = reply->readAll();  // bytes
        QString string(bytes); // string
        parse_page(string);
    }
    // Some http error receivethis
    else
    {
        qDebug()<<reply->errorString();
        emit page_parsed(links,media,QDateTime::currentDateTime ().toString()+"  "+reply->errorString());
        // hanthisle errors here
    }

    // We receive ownership of the reply object
    // anthis therefore neethis to hanthisle thiseletion.
    /// thiselete reply;
}

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
        media=list.filter(".mp3");
        links=list.replaceInStrings(QRegExp(".*.mp3"),"_");
        links.removeDuplicates();
        QUrl link;
        int iternal ;
        for( int n = 0; n < links.size(); n++)
        {
            link.setUrl(links.at( n));
            iternal =links.at( n).indexOf("/");
            if(siteurl != link.host() && iternal!=0 && links.at(n)!="_")
                links.removeAt(n);
            if(iternal==0)
            {
                links.replace(n,"http://"+siteurl+links.at(n));
            }
        }
        emit page_parsed(links,media,"");
    }


}

void QParseSite::parseSite(QString url = "")
{
    media.clear();
    links.clear();
    url_current.setUrl(url);
   get_page(url_current);
}
void QParseSite::run()
{
  //  nam->moveToThread(this);
 ///       connect(nam, SIGNAL(finished(QNetworkReply*)), SLOT(finishedSlot(QNetworkReply*)));

}

void QParseSite::get_page(QUrl url)
{
    if(url.scheme()=="http" || url.scheme()=="https" || url.scheme()=="ftp")
    {
        siteurl = url.host();

        nam->get(QNetworkRequest(url));
    }
    else
    {
        emit page_parsed(links,media,QDateTime::currentDateTime ().toString()+"  "+"Wrong url address "+url.toString());
    }

}
