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
        emit page_parsed(links,media,"",QDateTime::currentDateTime ().toString()+"  "+reply->errorString());
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
        links.clear();
        links=list.replaceInStrings(QRegExp(".*.mp3"),"_");
        links=list.replaceInStrings(QRegExp(".*.m3u"),"_");
        links=list.replaceInStrings(QRegExp("javascript:;"),"_");

        links.removeDuplicates();
        QUrl link("");
        int iternal ;
        QString url;
        for( int n = 0; n < links.size(); n++)
        {
            link.setUrl(links.at( n));
            iternal =links.at(n).indexOf("/");
            if(siteurl.host() != link.host())
            {
                if(iternal==1 || iternal==0)
                {
                    url = links.at(n);
                    if(iternal == 1)
                    {
                        url=url.replace("./","/");
                    }

                    /*
                    if(link.host()=="")
                    {
                        ///url = links.at(n);
                        if(url_current.toString().lastIndexOf("/") == url_current.toString().size()-1 &&  url.at(0)=='/')
                        {
                            url.replace(0,1,"");
                        }
                        url=url_current.toString()+url;
                    }
                    else
                          {
                              */
                              url = siteurl.scheme()+"://"+siteurl.host()+url;



                    links.replace(n,url);
                }
                else
                {
                    links.removeAt(n);
                }

            }



        }
        emit page_parsed(links.filter(siteurl.host()),media,content,"");
    }


}

void QParseSite::parseSite(QString url = "")
{
    media.clear();
    links.clear();
    url_current.setUrl(url,QUrl::TolerantMode);
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
        nam->get(QNetworkRequest(url));
    }
    else
    {
        emit page_parsed(links,media,"",QDateTime::currentDateTime ().toString()+"  "+tr("Wrong url address ")+url.toString());
    }

}
