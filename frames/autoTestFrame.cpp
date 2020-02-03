#include "autoTestFrame.h"
#include <QFileDialog>
#include <QMessageBox>

autoTestFrame::autoTestFrame(QWidget* parent)
    : QWidget(parent, Qt::Window)
{
    ui.setupUi(this);
    parent_ptr = parent;

    // deal with table
    atsManager = new AtsManager(this, ui.table);
    this->initTableColumnWidth();
    this->initTableConnection();

    //Load [current] settings
    this->loadConfig(Settings::Section::AUTOTEST_CURRENT);

    // decide enable table or read only
    this->enableTable(ui.ckbEnableEdit->isChecked());

    // load default script
    const QString path = ui.editScriptPath->text();
    if (!path.isEmpty() && atsManager->exist(path))
        atsManager->open(path);

}

autoTestFrame::~autoTestFrame()
{
}

void autoTestFrame::selectClick()
{
    // get script path
    auto path = QFileDialog::getOpenFileName(
        this,
        "Open autotest script file...",
        ".\\",
        "Autotest Script (*.ats);;All files (*.*)");
    if (!path.isEmpty())
    {
        atsManager->open(path);
        ui.editScriptPath->setText(path);
    }
}

void autoTestFrame::initTableColumnWidth()
{
    ui.table->setColumnWidth(0, 70);
    ui.table->setColumnWidth(1, 250);
}

void autoTestFrame::initTableConnection()
{
    connect(ui.table, SIGNAL(itemChanged(QTableWidgetItem*)), 
            atsManager, SLOT(textChanged(QTableWidgetItem*)));
}

void autoTestFrame::loadConfig(Settings::Section section)
{
    Settings setter(section);
    using Keys = Settings::Keys;

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    auto txt = setter.getValue(Keys::TEST_SCRIPT_PATH, "").toString();
    ui.editScriptPath->setText(txt);

    ui.ckbEnableEdit->setChecked(
        setter.getValue(Keys::ENABLE_SCRIPT_EDIT, 0).toInt());
}

void autoTestFrame::saveConfig(Settings::Section section)
{
    Settings setter(section);
    using Keys = Settings::Keys;

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    setter.setValue(
        Keys::TEST_SCRIPT_PATH, ui.editScriptPath->text());

    setter.setValue(
        Keys::ENABLE_SCRIPT_EDIT, (int)ui.ckbEnableEdit->isChecked());
}

// update editItem and table according to user choices
void autoTestFrame::updateUserCmd()
{
    //QString txt = item_command + "(" + item_value1 + "," + item_value2 + ")";
    QString txt("");
    if (!item_command.isEmpty())
    {
        if (item_value2.isEmpty())
            txt = item_command + "(" + item_value1 + ")";
        else
            txt = item_command + "(" + item_value1 + "," + item_value2 + ")";
    }

    auto t = ui.table->item(ui.table->currentRow(), 1);
    ui.editItem->setText(txt);
    t->setText(txt);
}

void autoTestFrame::saveClick()
{
    if (atsManager->fileName.isEmpty())
        this->saveAsClick();
    else
        atsManager->save();
}

void autoTestFrame::saveAsClick()
{
    auto path = QFileDialog::getSaveFileName(
        this,
        "Save autotest script file...",
        ".\\",
        "Autotest Script (*.ats);;All files (*.*)");
    if (!path.isEmpty())
        atsManager->save(path);
}

void autoTestFrame::newClick()
{
    // not save yet
    if (atsManager->changes)
    {
        auto result = QMessageBox::question(
            this, "Not save yet", "Do you want to save?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);
        switch (result)
        {
        case QMessageBox::Save:
            this->saveClick();
            if (atsManager->changes)  // actually user not save...
                return;
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            return;
        }
    }

    atsManager->newTable();
    ui.editStep      ->clear();
    ui.editScriptPath->clear();
    ui.editItem      ->clear();
}

void autoTestFrame::updateLineNumber()
{
    auto row = ui.table->currentRow() + 1;
    ui.editStep->setText(QString::number(row));

    if (row > 0)
        ui.editItem->setText(ui.table->item(row - 1, 1)->text());
}

void autoTestFrame::moveUpClick()
{
    auto currentRow = ui.table->currentRow();
    if (currentRow < 0)return;     // nothing selected

    atsManager->moveUp();
}

void autoTestFrame::moveDownClick()
{
    auto currentRow = ui.table->currentRow();
    if (currentRow < 0)return;     // nothing selected

    atsManager->moveDown();
}

void autoTestFrame::addItem()
{
    atsManager->addItem();
}

void autoTestFrame::deleteItem()
{
    atsManager->deleteItem();
    ui.editStep->setText(
        QString::number(ui.table->currentRow() + 1));
}

void autoTestFrame::enableTable(bool isEnable)
{
    if (isEnable)    // enable all
    {
        ui.groupBox2  ->setEnabled(true);
        ui.btnMoveUp  ->setEnabled(true);
        ui.btnMoveDown->setEnabled(true);
        ui.btnDelete  ->setEnabled(true);
        ui.btnAddNew  ->setEnabled(true);
    }
    else             // disable groupbox but enable some
    {
        ui.groupBox2  ->setEnabled(false);
        ui.btnMoveUp  ->setEnabled(false);
        ui.btnMoveDown->setEnabled(false);
        ui.btnDelete  ->setEnabled(false);
        ui.btnAddNew  ->setEnabled(false);
    }
}

// content of editItem changed by USER
void autoTestFrame::editItemChange(const QString& str)
{
    auto row = ui.table->currentRow();
    auto t = ui.table->item(row, 1);
    t->setText(str);
}

// a choice in cbCommand is selected
void autoTestFrame::cmdActivated(const QString& str)
{
    item_command = str;
    this->updateUserCmd();
}

// the param type is selected 
void autoTestFrame::typeActivated(int index)
{
    auto p = (QComboBox*)sender();
    if (p == ui.cbType1)  // type1
    {
        ui.editValue1->clear();
        if (index == 1)  // LDF signal
        {
            ui.cbType1_2->setEnabled(true);
            ui.editValue1->setText(ui.cbType1_2->currentText());
        }
        else
            ui.cbType1_2->setEnabled(false);
    }
    else                  // type2
    {
        ui.editValue2->clear();
        if (index == 1)  // LDF signal
        {
            ui.cbType2_2->setEnabled(true);
            ui.editValue2->setText(ui.cbType2_2->currentText());
        }
        else
            ui.cbType2_2->setEnabled(false);
    }
}

// LDF subtype is selected
void autoTestFrame::subTypeActivated(const QString& str)
{
    auto p = (QComboBox*)sender();
    
    if (p == ui.cbType1_2)   // param1
        ui.editValue1->setText(str);
    else                      // param2
        ui.editValue2->setText(str);
}

// editValue changed
void autoTestFrame::editValueChange(const QString& str)
{
    auto p = (QLineEdit*)sender();
    if (p == ui.editValue1)  // param1
        item_value1 = str;
    else                     // param2
        item_value2 = str;

    this->updateUserCmd();
}

void autoTestFrame::loadDefaultConfig()
{
    this->loadConfig(Settings::Section::AUTOTEST_DEFAULT);
    
    // load script
    const QString path = ui.editScriptPath->text();
    if (!path.isEmpty() && atsManager->exist(path))
        atsManager->open(path);
}

void autoTestFrame::saveDefaultConfig()
{
    this->saveConfig(Settings::Section::AUTOTEST_DEFAULT);
}

// override the virtual function.
// Save config and show mainFrame again
void autoTestFrame::closeEvent(QCloseEvent* event)
{
    // not save yet
    if (atsManager->changes)
    {
        auto result = QMessageBox::question(
            this, "Not save yet", "Do you want to save?",
            QMessageBox::Save | QMessageBox::Discard | QMessageBox::Cancel,
            QMessageBox::Save);
        switch (result)
        {
        case QMessageBox::Save:
            this->saveClick();
            if (atsManager->changes)  // actually user not save...
            {
                event->ignore();
                return;
            }
            break;
        case QMessageBox::Discard:
            break;
        case QMessageBox::Cancel:
            event->ignore();
            return;
        }
    }
    
    //TODO: save current config
    this->saveConfig(Settings::Section::AUTOTEST_CURRENT);
    event->accept();
}