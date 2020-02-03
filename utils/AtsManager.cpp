#include "AtsManager.h"
#include <QFile>
#include <QTextStream>
#include <QTableWidgetItem>

AtsManager::AtsManager(QObject* parent, QTableWidget* t)
    : QObject(parent), table(t), fileName("") {}

AtsManager::~AtsManager(){}

bool AtsManager::open(const QString& file)
{
    QFile script(file);
    if(!script.open(QIODevice::ReadOnly|QIODevice::Text))
        return false;

    this->fileName = file;
    QTextStream in(&script);

    int row = 0;             // current row
    int fillRowCount = 40;   // enlarge 40 items at one time

    table->setRowCount(fillRowCount);
    while (!in.atEnd())
    {
        if (row == fillRowCount)   // enlarge the table
        {
            fillRowCount += 40;
            table->setRowCount(fillRowCount);
        }
        QString line = in.readLine();
        table->setItem(row, 0, new QTableWidgetItem(QString::number(row+1)));  // step
        table->setItem(row, 1, new QTableWidgetItem(line));
        ++row;
    }
    table->setRowCount(row);   // delete blank rows
    
    return true;
}

bool AtsManager::exist(const QString& file)
{
    return QFile::exists(file);
}

QString AtsManager::text() const
{
    //TODO: return full text
    return QString();
}

void AtsManager::save(const QString& file)
{
    // if input file name is empty, select existing file name
    const QString* name;
    if (file.isEmpty())
    {
        if (this->fileName.isEmpty())
            return;
        else
            name = &this->fileName;
    }
    else
        name = &file;

    QFile script(*name);
    if (!script.open(QIODevice::WriteOnly | QIODevice::Text))
        return;
    this->fileName = *name;
    QTextStream in(&script);

    // save each row to disk
    int rowCount = table->rowCount();
    for (int i = 0; i < rowCount; i++)
        in << table->item(i, 1)->text() << "\n";

    // reset changes flag and apperance
    this->changes = false;
    this->setHeaderStar(false);
}

// when the text of item changed
void AtsManager::textChanged(QTableWidgetItem* item)
{
    auto column = table->currentColumn();
    if (column == -1)return;     // new tab is click

    this->changes = true;
    this->setHeaderStar(true);
}

// set the column-th header with or without star
void AtsManager::setHeaderStar(bool set)
{
    const QString header[]{ QString("COMMAND"),QString("COMMAND*") };
    int j = (set ? 1 : 0);

    auto h = new QTableWidgetItem(header[j]);
    h->setTextAlignment(Qt::AlignLeft);
    table->setHorizontalHeaderItem(1, h);
}

void AtsManager::newTable()
{
    fileName.clear();
    changes = false;
    table->clearContents();
    table->setRowCount(0);

    setHeaderStar(false);
}

// move current item up
void AtsManager::moveUp()
{
    int currentRow = table->currentRow();
    if (currentRow == 0)return;    // the first item cannot move up

    auto curItem = table->item(currentRow, 1);
    auto upItem  = table->item(--currentRow, 1);
    QString temp(curItem->text());
    //
    curItem->setText(upItem->text());
    upItem->setText(temp);

    table->selectRow(currentRow);   // select the moved up row
}

// move current item down
void AtsManager::moveDown()
{
    int rows = table->rowCount() - 1;
    int currentRow = table->currentRow();
    if (currentRow == rows)return;    // the last item cannot move down

    auto curItem = table->item(currentRow, 1);
    auto downItem = table->item(++currentRow, 1);
    QString temp(curItem->text());
    //
    curItem->setText(downItem->text());
    downItem->setText(temp);

    table->selectRow(currentRow);   // select the moved down row
}

void AtsManager::deleteItem()
{
    int currentRow = table->currentRow();
    if (currentRow < 0)return;      // nothing to delete
    table->removeRow(currentRow);
    table->selectRow(currentRow);   // select the current row

    // minus one for all line numbers below
    int rows = table->rowCount();
    while (currentRow < rows)
    {
        table->item(currentRow, 0)->setText(QString::number(currentRow + 1));
        ++currentRow;
    }
}

void AtsManager::addItem()
{
    int currentRow = table->currentRow();
    table->insertRow(++currentRow);
    table->setItem(currentRow, 0,
        new QTableWidgetItem(QString::number(currentRow + 1)));
    table->setItem(currentRow, 1, new QTableWidgetItem(""));
    table->selectRow(currentRow);   // select the added row

    // add one for all line numbers below
    int rows = table->rowCount();
    ++currentRow;
    while (currentRow < rows)
    {
        table->item(currentRow, 0)->setText(QString::number(currentRow + 1));
        ++currentRow;
    }
}
