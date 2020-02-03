#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_mainFrame.h"
#include "setupFrame.h"
#include "programFrame.h"
#include "controlFrame.h"
#include "diagnosisFrame.h"
#include "autoTestFrame.h"

class mainFrame : public QMainWindow
{
    Q_OBJECT

public:
    mainFrame(QWidget *parent = Q_NULLPTR);
    ~mainFrame();

private slots:
    void showSetupFrame();    // for btnSetup clicked
    void showProgramFrame();  // for btnProgram clicked
    void showControlFrame();  // for btnControl clicked
    void showDiagnosisFrame();// for btnDiagnosis clicked
    void showAutoTestFrame(); // for btnAutoTest clicked

private:
    Ui::mainFrameWindow ui;
    setupFrame*     setupFrm     = nullptr;
    programFrame*   programFrm   = nullptr;
    controlFrame*   controlFrm   = nullptr;
    diagnosisFrame* diagnosisFrm = nullptr;
    autoTestFrame*  autotestFrm  = nullptr;
};
