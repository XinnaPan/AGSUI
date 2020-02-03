#pragma once

/* *************************************************
 *  This frame is the review frame for program frame.
 * It can be reused as NVM or Production data review.
 * *************************************************/

#include <QWidget>
#include "ui_reviewFrame.h"
#include "ReviewFileParser.h"

enum class ReviewFrameType :int
{
    NVM,
    ProductionData
};

class reviewFrame : public QWidget
{
    Q_OBJECT

public:
    reviewFrame(QWidget *parent,
                ReviewFrameType type);
    ~reviewFrame();

private:
    Ui::reviewFrame ui;
    ReviewFrameType reviewType;      // describe this frame is NVM or Production data
    ReviewFileParser* table;         // Self defined class representing the table
    //
    void initTitleToProduction();    // init change button title, default is NVM
    void initColumnWidth();          // init proper column width
    //
    void connectUpdateCkbToSlot();   // connect update checkbox to `updateClicked()`

private slots:
    void loadFile();                  // load file and fill table
    void saveFile();                  // save file to disk
    //
    void updateClicked(bool isChecked);// deal with update checkbox click
};
