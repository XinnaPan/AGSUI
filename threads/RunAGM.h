#pragma once

/* *********************************************
 *  This class is used in control frame, dealing
 * with running AGM. 
 * @Date: 2020/01/29
 * *********************************************/

#include <QObject>

class RunAGM : public QObject
{
    Q_OBJECT

public:
    RunAGM(int AGMx, QObject* parent = nullptr);

public slots:
    //TODO: implement it and pass some args
    void runOnce(int AGMx);             // run AGM once

    //TODO: implement it and pass some args
    void runPeriodically(int AGMx);     // run AGM periodically

    //TODO: stop running periodically. May need timer
    void stop(int AGMx);

signals:
    void enableGroupBox(int AGMx, bool True);  // enable the corresponding groupbox

private:
    const int AGMx;    // whether AGM1 or AGM2
};
