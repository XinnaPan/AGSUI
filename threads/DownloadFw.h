#pragma once

/* *********************************************
 *  This class is used in program frame, dealing
 * with issues of downloading firmware to DUT.
 * @Date: 2020/01/29
 * *********************************************/

#include <QObject>

class DownloadFw : public QObject
{
    Q_OBJECT

public:
    DownloadFw(QObject* parent = nullptr);

public slots:
    //TODO: NOT implemented.And pass some args
    void download();        // download firmware to DUT

signals:
    void update(QString text, int value); // update GUI with 'text' and 'value'

private:
    const static QString statusHints[];   // hints for download status
};
