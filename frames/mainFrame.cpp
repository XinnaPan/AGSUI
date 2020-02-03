#include "mainFrame.h"

mainFrame::mainFrame(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);
}

mainFrame::~mainFrame()
{
}


// For btnSetup clicked
void mainFrame::showSetupFrame()
{
    setupFrm = new setupFrame(this);
    setupFrm->setAttribute(Qt::WA_DeleteOnClose);
    setupFrm->show();
}

// For btnProgram clicked
void mainFrame::showProgramFrame()
{
    programFrm = new programFrame(this);
    programFrm->setAttribute(Qt::WA_DeleteOnClose);
    programFrm->show();
}

void mainFrame::showControlFrame()
{
    controlFrm = new controlFrame(this);
    controlFrm->setAttribute(Qt::WA_DeleteOnClose);
    controlFrm->show();
}

void mainFrame::showDiagnosisFrame()
{
    diagnosisFrm = new diagnosisFrame(this);
    diagnosisFrm->setAttribute(Qt::WA_DeleteOnClose);
    diagnosisFrm->show();
}

void mainFrame::showAutoTestFrame()
{
    autotestFrm = new autoTestFrame(this);
    autotestFrm->setAttribute(Qt::WA_DeleteOnClose);
    autotestFrm->show();
}
