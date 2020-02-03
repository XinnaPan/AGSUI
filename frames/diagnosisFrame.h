#pragma once

#include <QWidget>
#include <QCloseEvent>
#include "ui_diagnosisFrame.h"
#include "Settings.h"

class diagnosisFrame : public QWidget
{
    Q_OBJECT

public:
    diagnosisFrame(QWidget *parent = Q_NULLPTR);
    ~diagnosisFrame();

private:
    Ui::diagnosisFrame ui;
    QWidget* parent_ptr;           // parent window pointer
    QString scriptPath;            // path of the script file
    //
    void loadConfig(Settings::Section section);   // load current/default configuration ini
    void saveConfig(Settings::Section section);   // save current/default configuration ini

private slots:
    void loadDefaultConfig();               // load default configuration
    void saveDefaultConfig();               // save default configuration
    //
    void runScriptClick();                  // btnRunScript clicked

protected:
    virtual void closeEvent(QCloseEvent* event) override;
};
