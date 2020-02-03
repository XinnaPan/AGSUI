#include "ReviewFileParser.h"
#include <QHboxLayout>
#include <QCheckBox>
#include <QBrush>
#include <QColor>

ReviewFileParser::ReviewFileParser(const QString& filePath, ParserType type)
{
    this->type = type;
    if (type == ParserType::NVM)
        this->loadNVMfile(filePath);
    else
        this->loadProductDataFile(filePath);
}

ReviewFileParser::~ReviewFileParser()
{
}

// change the row-th "NewValue" item to state `isEnable`
void ReviewFileParser::changeNewValState(int row, bool isEnable)
{
    static const QBrush disableBrush(QColor(220, 220, 220));
    static const QBrush enableBrush (QColor(255, 255, 255));

    auto p = newValues[row];
    if (isEnable)
    {
        p->setBackground(enableBrush);
        p->setFlags(p->flags() | (Qt::ItemIsEditable));
    }
    else
    {
        p->setBackground(disableBrush);
        p->setFlags(p->flags() & (~Qt::ItemIsEditable));
        p->setText("");
    }
}

void ReviewFileParser::saveFile(const QString& filePath)
{
    if (this->type == ParserType::NVM)
        this->saveNVMfile(filePath);
    else
        this->saveProductDataFile(filePath);
}

// TODO: NOT implemented. See requirement
void ReviewFileParser::loadNVMfile(const QString& filePath)
{
    //******Demo******
    fake_data();
}

// TODO: NOT implemented. See requirement
void ReviewFileParser::saveNVMfile(const QString& filePath)
{
    //******Demo******
    fake_output();
}

// TODO: NOT implemented. See requirement
void ReviewFileParser::loadProductDataFile(const QString& filePath)
{
    //******Demo******
    fake_data();
}

// TODO: NOT implemented. See requirement
void ReviewFileParser::saveProductDataFile(const QString& filePath)
{
    //******Demo******
    fake_output();
}


//******Demo******
#include <QDebug>
#include <vector>
using namespace std;
void ReviewFileParser::fake_data()
{
    rowCount = 5;
    static vector<QString> names{
        QString("string a"),
        QString("string b"),
        QString("string c"),
        QString("string d"),
        QString("string e")
    };
    static vector<QString> values{
        QString("123"),
        QString("456"),
        QString("789"),
        QString("101"),
        QString("112")
    };

    // Core part!!!
    // Add items and align center
    QTableWidgetItem* item;
    const QBrush brush(QColor(220, 220, 220));
    for (int i = 0; i < rowCount; i++)
    {
        item = new QTableWidgetItem(QString::number(i + 1));
        item->setTextAlignment(Qt::AlignCenter);
        numbers.append(item);
        //
        item = new QTableWidgetItem(names[i]);
        item->setTextAlignment(Qt::AlignCenter);
        signalNames.append(item);
        //
        item = new QTableWidgetItem(values[i]);
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable)); // not editable
        oldValues.append(item);
        //
        item = new QTableWidgetItem("");
        item->setTextAlignment(Qt::AlignCenter);
        item->setFlags(item->flags() & (~Qt::ItemIsEditable));  // not editable
        item->setBackground(brush);
        newValues.append(item);
        //
        auto widget = new QWidget();
        auto hbox = new QHBoxLayout(widget);
        auto ckb = new QCheckBox(widget);
        hbox->addWidget(ckb);
        hbox->setMargin(0);
        hbox->setAlignment(ckb, Qt::AlignCenter);
        widget->setLayout(hbox);
        isUpdate.append(widget);
    }
}
//
void ReviewFileParser::fake_output()
{
    // Core part!!
    for (int i = 0; i < rowCount; i++)
    {
        qDebug() << numbers[i]->text() << " "
                 << signalNames[i]->text() << " ";
        auto ckb = (QCheckBox*)isUpdate[i]->children().at(1);
        qDebug() << ckb->isChecked() << " "
                 << oldValues[i]->text() << " "
                 << newValues[i]->text() << "\n";
    }
}
