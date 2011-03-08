#include "headers/downloadfile.h"

DownloadFile::DownloadFile(QObject *parent,QProgressBar *progress,QLabel *label,QLabel *dlabel) :
    QObject(parent), current_progress(progress), speed_label(label),downedsize(dlabel)
{
    nam = new QNetworkAccessManager();
    connect(nam, SIGNAL(finished(QNetworkReply*)), SLOT(finishedSlot(QNetworkReply*)));
}
#include <QDebug>
#include <QFile>
#include <QFileInfo>
void DownloadFile::finishedSlot(QNetworkReply* reply)
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
        QFileInfo *file_info = new QFileInfo(file_url.toString());

        QFile *file = new QFile(dir.path()+QDir::toNativeSeparators("/")+file_info->fileName());
        if(file->exists())
                file->remove();
         if (!file->open(QIODevice::WriteOnly)) {
             emit(DownloadMediaFinished(file->errorString(),false));
         }
         file->write(reply->readAll());
         file->flush();
         file->close();
         delete file;
         emit(DownloadMediaFinished("",true));
         speed_label->setText("0.0 bytes/sec");
    }
    // Some http error receivethis
    else
    {
            emit(DownloadMediaFinished(reply->errorString(),false));
    }
}



void DownloadFile::DownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
    double recived=bytesReceived;
    quint64 B=1;
    QString f;
    quint64  KB = 1024 * B; //kilobyte
    quint64  MB = 1024 * KB; //megabyte
    quint64  GB = 1024 * MB; //gigabyte
    if (recived>GB)
        downedsize->setText(f.sprintf("%1.2f GB",QVariant(recived/GB).toDouble()) );
    else
        if (recived> MB)
            downedsize->setText(f.sprintf("%1.2f MB",QVariant(recived/MB).toDouble()));
        else
            if (recived> KB)
                downedsize->setText(f.sprintf("%1.2f KB",QVariant(recived/KB).toDouble()));
            else
                downedsize->setText(f.sprintf("%1.2f bytes",QVariant(recived).toDouble()));

    double speed = bytesReceived * 1000.0 / downloadTime.elapsed();
        QString unit;
        if (speed < 1024) {
            unit = "bytes/sec";
        } else if (speed < 1024*1024) {
            speed /= 1024;
            unit = "kB/s";
        } else {
            speed /= 1024*1024;
            unit = "MB/s";
        }
        speed_label->setText(QString::fromLatin1("%1 %2")
                      .arg(speed, 3, 'f', 1).arg(unit)) ;
    current_progress->setMaximum(bytesTotal);
    current_progress->setValue(bytesReceived);

}

void DownloadFile::download(QString given_url)
{
    if(!given_url.isEmpty())
    {
        file_url.setUrl(given_url,QUrl::TolerantMode);
    }
    if(!file_url.isEmpty() && file_url.isValid())
    {
        downloadTime.start();
        downedsize->setText("0");
        Reply = nam->get(QNetworkRequest(file_url));
        connect(Reply,SIGNAL(downloadProgress(qint64,qint64)),this,SLOT(DownloadProgress(qint64,qint64)));

    }
    else
    {
        emit(DownloadMediaFinished("Invalid url",false));
    }
}
