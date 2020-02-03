#include "programFrame.h"
#include "reviewFrame.h"
#include <QPixmap>
#include <QFileDialog>
#include <QDateTime>
#include "DUTinfoValidator.h"
#include "DownloadFw.h"

programFrame::programFrame(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    ui.setupUi(this);
    parent_ptr = parent;

    //TODO: Load [program_current] config. Display level.
    this->loadConfig(Settings::Section::PROGRAM_CURRENT);

    // init download thread
    this->initDownloadThread();
}

programFrame::~programFrame()
{
    threadDownload.quit();
    threadDownload.wait();
}

// override the virtual function.
// Save config and show mainFrame again
void programFrame::closeEvent(QCloseEvent* event)
{
    // save current config
    this->saveConfig(Settings::Section::PROGRAM_CURRENT);

    event->accept();
}

// connect to device
void programFrame::connectDUT()
{
    //TODO: call connect to device function

    //TODO: judge connect status 
    //@ if (Successful)
    //@     Change `connectionStatus`
    //@     Change button text & status text & img
    //@     Show HW/SW/Production date/Serial info
    //@     if (HW/Production info valid)
    //@         Copy to section2
    //@         Set new HW/Production img to yes.png
    //@     if (SW info valid)
    //@         Set new SW img to yes.png
    //@     else
    //@         Set new SW img to exclamation.png
    //@     if (Serial info valid)
    //@         Copy to section2
    //@         Set new Serial img to yes.png
    //@     else
    //@         Read serial from .ini
    //@         Set new Serial img to yes.png if not NULL
    //@ elseif (Connection timeout)
    //@     Change status text
    //@ elseif (Invalid device)
    //@     Change status text
    //@ elseif (Device unsupported)
    //@     Change status text
    //******demo******
    this->connectionStatus = true;
    ui.btnConnect->setText("Disconnect");
    ui.labelDUT_status->setText("connected");
    ui.labelDUT_light->setPixmap(QPixmap(":/programFrame/connected.png"));
    //
    const QString HW_version("0H07");
    const QString SW_version("0072");
    const QString ProductDate("181215");
    const QString SerialNum("1234");
    ui.editHWversion  ->setText(HW_version);
    ui.editSWversion  ->setText(SW_version);
    ui.editProductDate->setText(ProductDate);
    ui.editSerial     ->setText(SerialNum);
    //
    if (DUTinfoValidator::vldHWversion(HW_version))
    {
        ui.editNewHWversion->setText(HW_version);
        ui.labelHW_status->setPixmap(QPixmap(":/programFrame/yes.png"));
    }
    if (DUTinfoValidator::vldProductDate(ProductDate))
    {
        ui.editNewProductDate->setText(ProductDate);
        ui.labelProduct_status->setPixmap(QPixmap(":/programFrame/yes.png"));
    }
    if (!SW_version.isEmpty() && !ui.editNewSWversion->text().isEmpty())
    {
        if (DUTinfoValidator::vldSWversion(SW_version, ui.editNewSWversion->text()))
            ui.labelSW_status->setPixmap(QPixmap(":/programFrame/yes.png"));
        else
            ui.labelSW_status->setPixmap(QPixmap(":/programFrame/exclamation.png"));
    }
    if (DUTinfoValidator::vldSerial(SerialNum))
    {
        ui.editNewSerial->setText(SerialNum);
        ui.labelSerial_status->setPixmap(QPixmap(":/programFrame/yes.png"));
    }
    else
    {
        // Read serial from .ini
        // Set new Serial img to yes.png if not NULL
        Settings setter(Settings::Section::PROGRAM_CURRENT);
        auto temp = setter.getValue(
            Settings::Keys::LAST_SERIAL, QVariant());
        if (temp.isValid())
        {
            ui.editNewSerial->setText(temp.toString());
            this->increaseSerial();  // add 1
            ui.labelSerial_status->setPixmap(QPixmap(":/programFrame/yes.png"));
        }
    }
}

// Disconnect device
void programFrame::disconnectDUT()
{
    //TODO: call connect to device function

    this->connectionStatus = false;
    ui.btnConnect->setText("Connect");
    ui.labelDUT_status->setText("disconnected");
    ui.labelDUT_light->setPixmap(QPixmap(":/programFrame/disconnected.png"));
    ////
    ui.editHWversion  ->clear();
    ui.editSWversion  ->clear();
    ui.editProductDate->clear();
    ui.editSerial     ->clear();
}

void programFrame::loadConfig(Settings::Section section)
{
    Settings setter(section);

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    auto path = setter.getValue(Settings::Keys::FILE_PATH, QVariant());
    if (path.isValid())
    {
        auto pathS = path.toString();
        ui.editFilePath->setText(pathS);
        ui.editNewSWversion->setText(readSWversion(pathS));
    }

    ui.ckbApplication->setChecked(
        setter.getValue(Settings::Keys::DOWNLD_SEL_APP, 0).toInt());

    ui.ckbNVM->setChecked(
        setter.getValue(Settings::Keys::DOWNLD_SEL_NVM, 0).toInt());

    ui.ckbProductData->setChecked(
        setter.getValue(Settings::Keys::DOWNLD_SEL_PDATA, 0).toInt());
}

void programFrame::saveConfig(Settings::Section section)
{
    Settings setter(section);

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    auto path = ui.editFilePath->text();
    if (!path.isEmpty())
    {
        setter.setValue(
            Settings::Keys::FILE_PATH, path);
    }

    setter.setValue(
        Settings::Keys::DOWNLD_SEL_APP, (int)ui.ckbApplication->isChecked());

    setter.setValue(
        Settings::Keys::DOWNLD_SEL_NVM, (int)ui.ckbNVM->isChecked());

    setter.setValue(
        Settings::Keys::DOWNLD_SEL_PDATA, (int)ui.ckbProductData->isChecked());
}

// create and connect for download thread
void programFrame::initDownloadThread()
{
    auto dl = new DownloadFw;
    dl->moveToThread(&threadDownload);

    connect(&threadDownload, &QThread::finished, dl, &QObject::deleteLater);
    //
    connect(this, SIGNAL(startDownload()), dl, SLOT(download()));
    connect(dl, SIGNAL(update(QString, int)), this, SLOT(updateProgressBar(QString, int)));

    threadDownload.start();
}

// Read SW version from firmware
QString programFrame::readSWversion(const QString& file)
{
    //TODO:NOT Implemented!!
    //******demo******
    return QString("0102");
}


// --------------------- slots -------------------------
// btnConnect clicked
void programFrame::btnConnect_clicked()
{
    if (!connectionStatus)
        this->connectDUT();
    else
        this->disconnectDUT();
}

// Get firmware file path
void programFrame::getFilePath()
{
    auto path = QFileDialog::getOpenFileName(
        this,
        "Open firmware file...",
        ".\\",
        "DUT Firmware (*.bin);;All files (*.*)");
    if (!path.isEmpty())
    {
        ui.editFilePath->setText(path);

        //Read SW version from firmware
        ui.editNewSWversion->setText(readSWversion(path));
    }
}

// Get date of today
void programFrame::getToday()
{
    QDateTime current = QDateTime::currentDateTime();
    ui.editNewProductDate->setText(current.toString("yyMMdd"));
}

// Download button clicked
void programFrame::downloadClicked()
{
    // Auto Increase
    bool autoIncrease = ui.ckbAutoInc->isChecked();
    if (autoIncrease)
    {
        Settings setter(Settings::Section::PROGRAM_CURRENT);
        setter.setValue(
            Settings::Keys::LAST_SERIAL, ui.editNewSerial->text());
    }

    // reset status
    ui.labelProgress->setText("Ready");
    ui.pbarProgress->setValue(0);

    //TODO: Download firmware if connected
    //@if(this->connectionStatus)
    emit startDownload();

    //TODO: Add record to file `prog_history.log`

}

void programFrame::updateProgressBar(QString text, int value)
{
    ui.labelProgress->setText(text);
    ui.pbarProgress->setValue(value);
}

void programFrame::loadDefaultConfig()
{
    this->loadConfig(Settings::Section::PROGRAM_DEFAULT);
}

void programFrame::saveDefaultConfig()
{
    this->saveConfig(Settings::Section::PROGRAM_DEFAULT);
}

// new HW version edit changed
void programFrame::HWversionChanged(const QString& text)
{
    if (text.isEmpty())
        ui.labelHW_status->clear();
    else if (DUTinfoValidator::vldHWversion(text))
        ui.labelHW_status->setPixmap(QPixmap(":/programFrame/yes.png"));
    else
        ui.labelHW_status->setPixmap(QPixmap(":/programFrame/exclamation.png"));
}

void programFrame::SWversionChanged(const QString& text)
{
    if (text.isEmpty())
        ui.labelSW_status->clear();
    else if (DUTinfoValidator::vldSWversion(ui.editSWversion->text(), text))
        ui.labelSW_status->setPixmap(QPixmap(":/programFrame/yes.png"));
    else
        ui.labelSW_status->setPixmap(QPixmap(":/programFrame/exclamation.png"));
}

void programFrame::proDateChanged(const QString& text)
{
    if (text.isEmpty())
        ui.labelProduct_status->clear();
    else if (DUTinfoValidator::vldProductDate(text))
        ui.labelProduct_status->setPixmap(QPixmap(":/programFrame/yes.png"));
    else
        ui.labelProduct_status->setPixmap(QPixmap(":/programFrame/exclamation.png"));
}

void programFrame::serialChanged(const QString& text)
{
    if (text.isEmpty())
        ui.labelSerial_status->clear();
    else if (DUTinfoValidator::vldSerial(text))
        ui.labelSerial_status->setPixmap(QPixmap(":/programFrame/yes.png"));
    else
        ui.labelSerial_status->setPixmap(QPixmap(":/programFrame/exclamation.png"));
}

// Increase the serial number
void programFrame::increaseSerial()
{
    auto original = ui.editNewSerial->text();
    if (DUTinfoValidator::vldSerial(original))
    {
        int value = original.toInt();
        if (value < 9)
            ui.editNewSerial->setText("000" + QString::number(value + 1));
        else if (value < 99)
            ui.editNewSerial->setText("00" + QString::number(value + 1));
        else if (value < 999)
            ui.editNewSerial->setText("0" + QString::number(value + 1));
        else if (value < 9999)
            ui.editNewSerial->setText(QString::number(value + 1));
    }
}

// decrease the serial number
void programFrame::decreaseSerial()
{
    auto original = ui.editNewSerial->text();
    if (DUTinfoValidator::vldSerial(original))
    {
        uint value = original.toUInt();
        if (value > 1000)
            ui.editNewSerial->setText(QString::number(value - 1));
        else if (value > 100)
            ui.editNewSerial->setText("0" + QString::number(value - 1));
        else if (value > 10)
            ui.editNewSerial->setText("00" + QString::number(value - 1));
        else if (value > 0)
            ui.editNewSerial->setText("000" + QString::number(value - 1));
    }
}

// show NVM data review frame
void programFrame::showNVMreview()
{
    auto reviewFrm = new reviewFrame(this, ReviewFrameType::NVM);
    reviewFrm->setAttribute(Qt::WA_DeleteOnClose);
    reviewFrm->show();
}

// show production data review frame
void programFrame::showProductionReview()
{
    auto reviewFrm = new reviewFrame(this, ReviewFrameType::ProductionData);
    reviewFrm->setAttribute(Qt::WA_DeleteOnClose);
    reviewFrm->show();
}
