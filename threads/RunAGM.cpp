#include "RunAGM.h"
#include <QThread>

RunAGM::RunAGM(int AGMx, QObject* parent) :
    QObject(parent), AGMx(AGMx) {}

// --------------------- SLOT ---------------------
// run AGM once
void RunAGM::runOnce(int AGMx)
{
    if (AGMx != this->AGMx) return;

    //TODO: call the AGM running function
    //******Demo******
    QThread::sleep(2);

    emit enableGroupBox(AGMx, true);
}

// run AGM periodically
void RunAGM::runPeriodically(int AGMx)
{
    if (AGMx != this->AGMx) return;

    //TODO: implement it. (May need timer)
}

void RunAGM::stop(int AGMx)
{
    if (AGMx != this->AGMx) return;

    //TODO: stop running periodically. May need timer
}
