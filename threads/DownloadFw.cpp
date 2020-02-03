#include "DownloadFw.h"

const QString DownloadFw::statusHints[]{
    QString("Ready"),
    QString("Initializing"),
    QString("Downloading Application"),
    QString("Downloading NVM"),
    QString("Downloading production data"),
    QString("Checking"),
    QString("Resetting DUT"),
    QString("Finished")
};


DownloadFw::DownloadFw(QObject *parent)
    : QObject(parent){}

// download firmware to DUT
#include <QThread>
void DownloadFw::download()
{
    //TODO: download
    //******demo******
    for (int i = 0; i <= 100; i++)
    {
        emit update(statusHints[i / 13], i);
        QThread::msleep(100);
    }
}