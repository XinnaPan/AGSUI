#pragma once

#include <QWidget>
#include <QCloseEvent>
#include <QThread>
#include <QTimer>
#include "ui_controlFrame.h"
#include "Settings.h"

class controlFrame : public QWidget
{
    Q_OBJECT

public:
    enum class PowerType :int
    {
        DUT_POWER,
        OVER_CURRENT,
        EXT_DC_ADP,
        TYPE_C_9V
    };

    controlFrame(QWidget *parent = Q_NULLPTR);
    ~controlFrame();

private:
    Ui::controlFrame ui;
    QWidget* parent_ptr;           // parent window pointer
    QTimer powerStatusTimer;       // timer for power status, connect checkPowerStatus()
    QThread AGM1_thread;           // thread associated with AGM1 running
    QThread AGM2_thread;           // thread associated with AGM1 running
    //
    bool isAGM1running = false;    // whether AGM is running
    bool isAGM2running = false;
    //
    void initAGM_thread();         // create and move the object to new thread
    void setImgForPowerStatus(PowerType type, bool lightUp);
    void setValidatorForPos();     // set validator for pos edit
    void setValidatorForTime();    // set validator for time interval
    //
    void loadConfig(Settings::Section section);   // load current/default configuration ini
    void saveConfig(Settings::Section section);   // save current/default configuration ini

private slots:
    void loadDefaultConfig();               // load default configuration
    void saveDefaultConfig();               // save default configuration
    //
    void sliderPos1_1_changed(int value);   // POS slider£¬ AGM1
    void sliderPos1_2_changed(int value);
    void sliderPos2_1_changed(int value);   // POS slider£¬ AGM2
    void sliderPos2_2_changed(int value);
    //
    void setEnableAGMx_widgets(int AGMx, bool isEnable);
    void checkPowerStatus();                // for timer
    //
    void posEditing(const QString& text);   // change the pos slider when editing line box
    void changeRunText(bool isCheck);       // ckbAutoRun change btnRun's text
    void btnRunClicked();                   // button Run is clicked

signals:
    void runAGM_once(int AGMx);             //TODO: some args may need to be passed
    void runAGM_periodically(int AGMx);     //TODO: some args may need to be passed
    void stopAGM_running(int AGMx);         // stop it from running periodically

protected:
    virtual void closeEvent(QCloseEvent* event) override;
};
