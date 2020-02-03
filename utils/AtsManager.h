#pragma once

/* ***************************************************
 *  Auto test script manger, including script load and
 * save, display and basic operation of items(such as 
 * move up and down, delete etc.), plain text operation
 * (such as get and a certain line, get and set whole
 * text). The compiler need to implemented.
 * @Date: 2020/01/31
 * ****************************************************/


#include <QObject>
#include <QTableWidget>


class AtsManager : public QObject
{
    Q_OBJECT

public:
    QString fileName;
    bool changes = false;   // is any change since last save

public:
    AtsManager(QObject *parent, QTableWidget* t);
    ~AtsManager();
    //
    bool open(const QString& file);      // open file, return ture if open successfully
    bool exist(const QString& file);     // is file exist
    void save(const QString& file = QString()); // save file
    void newTable();                     // create a new table
    void moveUp();                       // move current item up
    void moveDown();                     // move current item down
    void deleteItem();                   // delete current item
    void addItem();                      // add a new item below
    //
    QString text() const;                //TODO: get the current full text

public slots:
    void textChanged(QTableWidgetItem* item);  // when the text of item changed

private:
    QTableWidget* table;
    //
    void setHeaderStar(bool set);   // set the COMMAND header with or without star
};
