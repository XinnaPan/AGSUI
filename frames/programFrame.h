#pragma once

#include <QWidget>
#include <QCloseEvent>
#include <QThread>
#include "ui_programFrame.h"
#include "Settings.h"

class programFrame : public QWidget
{
    Q_OBJECT

public:
    programFrame(QWidget *parent = Q_NULLPTR);
    ~programFrame();

private:
    Ui::programFrame ui;
    QWidget* parent_ptr;            // parent window pointer
    QThread threadDownload;         // download thread
    bool connectionStatus = false;  // whether the device is connected
    //
    void connectDUT();              // connect to device
    void disconnectDUT();           // disconnect device
    //
    void loadConfig(Settings::Section section);   // load current/default configuration ini
    void saveConfig(Settings::Section section);   // save current/default configuration ini
    //
    void initDownloadThread();                    // create and connect for download thread
    QString readSWversion(const QString& file);   //TODO:NOT Implemented!! read SW version from firmware

private slots:
    void btnConnect_clicked();      // btnConnect clicked
    void getFilePath();             // get firmware file path
    void getToday();                // get date of today
    void loadDefaultConfig();       // load default configuration
    void saveDefaultConfig();       // save default configuration
    //
    void HWversionChanged(const QString& text); // new HW version edit changed
    void SWversionChanged(const QString& text); // new SW version edit changed
    void proDateChanged  (const QString& text); // new production date edit changed
    void serialChanged   (const QString& text); // new serial number edit changed
    //
    void increaseSerial();           // increase the serial number
    void decreaseSerial();           // decrease the serial number
    //
    void downloadClicked();          // download button clicked
    void updateProgressBar(QString text, int value); // text and bar
    //
    void showNVMreview();            // show NVM data review frame
    void showProductionReview();     // show production data review frame

signals:
    void startDownload();            // start download job

protected:
    virtual void closeEvent(QCloseEvent* event) override;
};
