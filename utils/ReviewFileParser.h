#pragma once

/* *****************************************************
 *  File Parser for NVM and Production data. It maintain
 * QWidget pointers in QList maintainer, and delete them
 * automatically when deconstruct.
 *@Date: 2020/1/25
 * *****************************************************/

#include <QWidget>
#include <QTableWidgetItem>
#include <QList>
#include <QString>

class ReviewFileParser
{
public:
    enum class ParserType :int
    {
        NVM,
        ProductionData
    };

    ReviewFileParser(const QString& filePath,
                     ParserType type);
    ~ReviewFileParser();
    void changeNewValState(int row, bool isEnable);
    void saveFile(const QString& filePath);

public:
    QList<QTableWidgetItem*> numbers;
    QList<QTableWidgetItem*> signalNames;
    QList<QWidget*>          isUpdate;
    QList<QTableWidgetItem*> oldValues;
    QList<QTableWidgetItem*> newValues;
    int rowCount = 0;

private:
    ParserType type;
    //
    void loadNVMfile(const QString& filePath);          // TODO: NOT implemented. See requirement
    void saveNVMfile(const QString& filePath);          // TODO: NOT implemented. See requirement
    void loadProductDataFile(const QString& filePath);  // TODO: NOT implemented. See requirement
    void saveProductDataFile(const QString& filePath);  // TODO: NOT implemented. See requirement
    //
    void fake_data();   // for demo
    void fake_output(); // for demo
};

