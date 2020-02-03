#include "reviewFrame.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QCheckBox>

reviewFrame::reviewFrame(QWidget *parent, ReviewFrameType type)
    : QWidget(parent, Qt::Window),
      reviewType(type),
      table(nullptr)
{
    ui.setupUi(this);

    // init title. Default is NVM so we don't judge it.
    if (type == ReviewFrameType::ProductionData)
        this->initTitleToProduction();
    this->initColumnWidth();

}

reviewFrame::~reviewFrame()
{
    if (table)
        delete table;
}

// Init change button title, default is NVM
void reviewFrame::initTitleToProduction()
{
    ui.labelFrameTitle->setText("Production\ndata review");
    ui.btnSaveFile->setText("Save\nPRD File");
    ui.btnLoadFile->setText("Load\nPRD File");
}

void reviewFrame::initColumnWidth()
{
    ui.tableWidget->setColumnWidth(0, 80);
    ui.tableWidget->setColumnWidth(1, 180);
    ui.tableWidget->setColumnWidth(3, 160);
    ui.tableWidget->setColumnWidth(4, 160);
}

// connect update checkbox to `updateClicked()`, so that
//we can send our self-defined signal.
void reviewFrame::connectUpdateCkbToSlot()
{
    for (auto widget : table->isUpdate)
    {
        connect(widget->children().at(1),
                SIGNAL(clicked(bool)),
                this,
                SLOT(updateClicked(bool)));
    }
}

//-------------------------- Slots ----------------------------
// load file and fill table
void reviewFrame::loadFile()
{
    // get file name
    auto path = QFileDialog::getOpenFileName(
        this,
        "Open NVM or production data file...",
        ".\\",
        "NVM or production data (*.csv);;All files (*.*)");
    if (path.isEmpty())
        return;

    // if table already loaded, delete it before a new one is created
    if (table) delete table;

    table = new ReviewFileParser(path, (ReviewFileParser::ParserType)reviewType);
    ui.tableWidget->setRowCount(table->rowCount);

    for (int row = 0; row < table->rowCount; row++)
    {
        ui.tableWidget->setItem(row, 0, table->numbers[row]);
        ui.tableWidget->setItem(row, 1, table->signalNames[row]);
        ui.tableWidget->setCellWidget(row, 2, table->isUpdate[row]);
        ui.tableWidget->setItem(row, 3, table->oldValues[row]);
        ui.tableWidget->setItem(row, 4, table->newValues[row]);
    }

    this->connectUpdateCkbToSlot();
}

// save file to disk
void reviewFrame::saveFile()
{
    // remind load file first
    if (!table)
    {
        QMessageBox::information(
            this,
            "Warning!",
            "You must load file first before saving it.",
            QMessageBox::Yes);
        return;
    }

    // get save file name
    auto path = QFileDialog::getSaveFileName(
        this,
        "Save NVM or production data file...",
        ".\\",
        "NVM or production data (*.csv);;All files (*.*)");
    if (path.isEmpty())
        return;

    table->saveFile(path);
}

// deal with update checkbox click and send self-defined signal
#include<QDebug>
void reviewFrame::updateClicked(bool isChecked)
{
    // decide check box of which row and its check state
    auto ckb = sender();
    int row = 0;
    for (auto p : table->isUpdate)
    {
        if (p->children().at(1) == ckb)
            break;
        else
            ++row;
    }

    table->changeNewValState(row, isChecked);
}

