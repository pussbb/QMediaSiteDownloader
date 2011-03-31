#ifndef DOWNLOADFILE_H
#define DOWNLOADFILE_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QUrl>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QProgressBar>
#include <QDir>
#include <QTime>
#include <QLabel>
class DownloadFile : public QObject
{
    Q_OBJECT
public:
    explicit DownloadFile(QObject *parent = 0, QProgressBar *progress = new QProgressBar,
                          QLabel *label = new QLabel,QLabel *dlabel = new QLabel);
    void inline setMediaPath(QString path){dir.setPath(path);};
    void download(QString given_url);
    QUrl file_url;
    QFile *file;
    QLabel *speed_label;
    QLabel *downedsize;
    QProgressBar *current_progress;
    QDir dir;
    bool is_downloading;
    void inline abort_download(){Reply->close();};
signals:
    void DownloadMediaFinished(QString error,bool finished);

public slots:
     void finishedSlot(QNetworkReply*)   ;
     void DownloadProgress(qint64 , qint64 );
     void httpReadyRead();
private:
     QNetworkAccessManager *dnam;
     QNetworkReply *Reply;
     QTime downloadTime;
};

#endif // DOWNLOADFILE_H
