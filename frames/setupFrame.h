#pragma once

#include <QWidget>
#include <QCloseEvent>
#include "ui_setupFrame.h"
#include "Settings.h"

class setupFrame : public QWidget
{
    Q_OBJECT

public:
    setupFrame(QWidget *parent = Q_NULLPTR);
    ~setupFrame();

private:
    Ui::setupFrame ui;
    QWidget* parent_ptr;           //parent window pointer
    //
    // data relative to the tools
    int voltage_mV = 7000;
    //
    QString getLDFfileName();                     // get the selected LDF file name
    void loadConfig(Settings::Section section);   // load current/default configuration ini
    void saveConfig(Settings::Section section);   // save current/default configuration ini

private slots:
    void changePin5(bool selectLin);      // change pin5 to LIN2 or PWM2
    void loadDefaultConfig();             // load default configuration
    void saveDefaultConfig();             // save default configuration
    //
    void changeVoltageDisplay(int value); // change editVoltage according to slider
    void voltageEditing(QString text);    // uncheck output box when editing
    void powerOutput(bool isChecked);     // output voltage power
    //
    void PWM1_freqEditing(QString text);  // uncheck output box when editing
    void PWM2_freqEditing(QString text);  // uncheck output box when editing
    void PWM1_output(bool isChecked);     // output PWM1 signal
    void PWM2_output(bool isChecked);     // output PWM2 signal
    //
    void displayLDF_fileName1();          // display name on 'cbLDFfilePath1'
    void displayLDF_fileName2();          // display name on 'cbLDFfilePath2'

protected:
    virtual void closeEvent(QCloseEvent* event) override;
};
