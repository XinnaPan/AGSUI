#pragma once

#include <QWidget>
#include <QCloseEvent>
#include "ui_autoTestFrame.h"
#include "AtsManager.h"
#include "Settings.h"

class autoTestFrame : public QWidget
{
    Q_OBJECT

public:
    autoTestFrame(QWidget *parent = Q_NULLPTR);
    ~autoTestFrame();

private:
    Ui::autoTestFrame ui;
    QWidget* parent_ptr;      // parent window pointer
    AtsManager* atsManager;   // manager for autotest script and its table
    //
    QString item_command;     // the selected command
    QString item_value1;      // user input value1
    QString item_value2;      // user input value2
    //
    void initTableColumnWidth(); // set table column width
    void initTableConnection();  // connect slots for AtsManager
    //
    void loadConfig(Settings::Section section);   // load current/default configuration ini
    void saveConfig(Settings::Section section);   // save current/default configuration ini
    //
    void updateUserCmd();       // update editItem and table according to user choices

private slots:
    void selectClick();       // btnSelect clicked
    void saveClick();         // btnSave
    void saveAsClick();       // btnSaveAs
    void newClick();          // btnNewClick
    void updateLineNumber();  // selected item change, update editStep
    //
    void moveUpClick();       // move current item up
    void moveDownClick();     // move current item down
    void addItem();           // add item below
    void deleteItem();        // delete current item
    //
    void enableTable(bool isEnable);     // ckbEnableEdit, whether enable table
    //
    void editItemChange(const QString&);  // content of editItem changed by user
    void cmdActivated(const QString&);    // a choice in cbCommand is selected
    void typeActivated(int index);        // the param type is selected 
    void subTypeActivated(const QString&);// LDF subtype is selected
    void editValueChange(const QString&); // editValue changed
    //
    void loadDefaultConfig();               // load default configuration
    void saveDefaultConfig();               // save default configuration

protected:
    virtual void closeEvent(QCloseEvent* event) override;
};
