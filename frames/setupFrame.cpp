#include <QMessageBox>
#include <QFileDialog>
#include "setupFrame.h"


// construct function
setupFrame::setupFrame(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    ui.setupUi(this);
    parent_ptr = parent;

    // TODO: Load [setup_current] config. Display level.
    this->loadConfig(Settings::Section::SETUP_CURRENT);

    // TODO: Enumerate available devices.

    // TODO: Send all widgets values to hardware.

}

setupFrame::~setupFrame()
{
}

// Get the selected LDF file name
QString setupFrame::getLDFfileName()
{
    return QFileDialog::getOpenFileName(
        this,
        "Open LDF file...",
        ".\\",
        "LDF files (*.ldf);;All files (*.*)");
}

void setupFrame::loadConfig(Settings::Section section)
{
    Settings setter(section);
    
    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    ui.ckbPowerEnable->setChecked(
        setter.getValue(Settings::Keys::POWER_EN, 0).toInt() );

    this->voltage_mV = setter.getValue(Settings::Keys::POWER_VOLT, 7000).toInt();
    ui.editVoltage->setText(QString::number(double(voltage_mV) / 1000, 'f', 1));
    ui.sliderVoltage->setSliderPosition(voltage_mV / 100);  //70~165

    ui.ckbLinEnable1->setChecked(
        setter.getValue(Settings::Keys::LIN_CH1_EN, 0).toInt() );

    int temp = setter.getValue(
        Settings::Keys::LIN_CH1_BAUD, 10417).toInt() == 10417 ? 0 : 1;
    ui.cbBaudrate1->setCurrentIndex(temp);

    const int APPEND_TO_END = 12;  //any integer larger than 10...
    const int stCh1LdfIdx = (int)Settings::Keys::CH1_LDF_FILE_HISTORY1;
    ui.cbLDFfilePath1->clear();    // clear box first
    for (int i = stCh1LdfIdx; i < stCh1LdfIdx + 10; i++)
    {
        auto value = setter.getValue(Settings::Keys(i), QVariant());
        bool isExist = value.isValid();
        if (isExist)
            ui.cbLDFfilePath1->insertItem(APPEND_TO_END, value.toString());
        else
            break;
    }

    ui.ckbPWMEnable1->setChecked(
        setter.getValue(Settings::Keys::PWM_CH1_EN, 0).toInt());

    QVariant tempQv = setter.getValue(Settings::Keys::PWM_CH1_FREQ, 100);
    temp = tempQv.toInt();
    if (temp % 100 == 0 && 1 <= temp / 100 && temp / 100 <= 5) // 100~500
        ui.cbPWMfreqency1->setCurrentIndex(temp / 100 - 1);
    else
    {
        ui.cbPWMfreqency1->insertItem(0, QString(" ") + tempQv.toString());
        ui.cbPWMfreqency1->setCurrentIndex(0);
    }

    auto tempQs = setter.getValue(Settings::Keys::PIN5_FUNC_SEL, QString("LIN2")).toString();
    tempQs == QString("LIN2") ?
        ui.rbLinCh2Sel->setChecked(true) : ui.rbPWMCh2Sel->setChecked(true);
    ui.labelPin5->setText(tempQs);
        
    ui.ckbLinEnable2->setChecked(
        setter.getValue(Settings::Keys::LIN_CH2_EN, 0).toInt());

    temp = setter.getValue(
        Settings::Keys::LIN_CH2_BAUD, 10417).toInt() == 10417 ? 0 : 1;
    ui.cbBaudrate2->setCurrentIndex(temp);

    const int stCh2LdfIdx = (int)Settings::Keys::CH2_LDF_FILE_HISTORY1;
    ui.cbLDFfilePath2->clear();    // clear box first
    for (int i = stCh2LdfIdx; i < stCh2LdfIdx + 10; i++)
    {
        auto value = setter.getValue(Settings::Keys(i), QVariant());
        bool isExist = value.isValid();
        if (isExist)
            ui.cbLDFfilePath2->insertItem(APPEND_TO_END, value.toString());
        else
            break;
    }

    ui.ckbPWMEnable2->setChecked(
        setter.getValue(Settings::Keys::PWM_CH2_EN, 0).toInt());

    tempQv = setter.getValue(Settings::Keys::PWM_CH2_FREQ, 100);
    temp = tempQv.toInt();
    if (temp % 100 == 0 && 1 <= temp / 100 && temp / 100 <= 5) // 100~500
        ui.cbPWMfreqency2->setCurrentIndex(temp / 100 - 1);
    else
    {
        ui.cbPWMfreqency2->insertItem(0, QString(" ") + tempQv.toString());
        ui.cbPWMfreqency2->setCurrentIndex(0);
    }
}

void setupFrame::saveConfig(Settings::Section section)
{
    Settings setter(section);

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    //TODO: change to actual download port
    setter.setValue(Settings::Keys::DOWNLOAD_PORT, "USB_HIDx");

    setter.setValue(
        Settings::Keys::POWER_EN, (int)ui.ckbPowerEnable->isChecked());

    setter.setValue(Settings::Keys::POWER_VOLT, this->voltage_mV);

    setter.setValue(
        Settings::Keys::LIN_CH1_EN, (int)ui.ckbLinEnable1->isChecked());

    setter.setValue(
        Settings::Keys::LIN_CH1_BAUD, 
        ui.cbBaudrate1->currentText().toInt());

    const int LDF_CH1_count = ui.cbLDFfilePath1->count();
    const int stCh1LdfIdx = (int)Settings::Keys::CH1_LDF_FILE_HISTORY1;
    for (int i = 0; i < LDF_CH1_count; i++)
    {
        setter.setValue(
            Settings::Keys(i + stCh1LdfIdx),
            ui.cbLDFfilePath1->itemText(i) );
    }

    setter.setValue(
        Settings::Keys::PWM_CH1_EN, (int)ui.ckbPWMEnable1->isChecked());

    setter.setValue(
        Settings::Keys::PWM_CH1_FREQ, 
        ui.cbPWMfreqency1->currentText().toInt());

    int temp = ui.buttonGroup->checkedId();
    QString tempQs = temp == -2 ? QString("LIN2") : QString("PWM2");
    setter.setValue(
        Settings::Keys::PIN5_FUNC_SEL, tempQs);

    setter.setValue(
        Settings::Keys::LIN_CH2_EN, (int)ui.ckbLinEnable2->isChecked());

    setter.setValue(
        Settings::Keys::LIN_CH2_BAUD, 
        ui.cbBaudrate2->currentText().toInt());

    const int LDF_CH2_count = ui.cbLDFfilePath2->count();
    const int stCh2LdfIdx = (int)Settings::Keys::CH2_LDF_FILE_HISTORY1;
    for (int i = 0; i < LDF_CH2_count; i++)
    {
        setter.setValue(
            Settings::Keys(i + stCh2LdfIdx),
            ui.cbLDFfilePath2->itemText(i));
    }

    setter.setValue(
        Settings::Keys::PWM_CH2_EN, (int)ui.ckbPWMEnable2->isChecked());

    setter.setValue(
        Settings::Keys::PWM_CH2_FREQ, 
        ui.cbPWMfreqency2->currentText().toInt());
}



// override the virtual function.
// Save config and show mainFrame again
void setupFrame::closeEvent(QCloseEvent* event)
{
    //save current config
    this->saveConfig(Settings::Section::SETUP_CURRENT);

    event->accept();
}

//-------------------- Slots -------------------------
// change pin5 to LIN2 or PWM2
void setupFrame::changePin5(bool selectLin)
{
    if (selectLin)
    {
        ui.labelPin5->setText("LIN2");
        ui.ckbLinEnable2  ->setEnabled(true);
        ui.cbBaudrate2    ->setEnabled(true);
        ui.labelLDFfile2  ->setEnabled(true);
        ui.cbLDFfilePath2 ->setEnabled(true);
        ui.btnLoadLDF2    ->setEnabled(true);
        //
        ui.ckbPWMEnable2  ->setEnabled(false);
        ui.cbPWMfreqency2 ->setEnabled(false);
    }
    else
    {
        ui.labelPin5->setText("PWM2");
        ui.ckbLinEnable2  ->setEnabled(false);
        ui.cbBaudrate2    ->setEnabled(false);
        ui.labelLDFfile2  ->setEnabled(false);
        ui.cbLDFfilePath2 ->setEnabled(false);
        ui.btnLoadLDF2    ->setEnabled(false);
        //
        ui.ckbPWMEnable2  ->setEnabled(true);
        ui.cbPWMfreqency2 ->setEnabled(true);
    }
}

// change editVoltage according to slider
void setupFrame::changeVoltageDisplay(int value)
{
    voltage_mV = value * 100;
    ui.editVoltage->setText(QString::number(double(voltage_mV) / 1000, 'f', 1));
}

// load default configuration
void setupFrame::loadDefaultConfig()
{
    this->loadConfig(Settings::Section::SETUP_DEFAULT);
}

// output voltage power
void setupFrame::powerOutput(bool isChecked)
{
    if (isChecked)
    {
        // validator for voltage input(positive float number limited)
        QRegExp regx("^(([0-9]+\\.[0-9]*[1-9][0-9]*)|([0-9]*[1-9][0-9]*\\.[0-9]+)|([0-9]*[1-9][0-9]*))$");
        auto voltage = ui.editVoltage->text();
        if (regx.exactMatch(voltage))
        {
            int integerVoltage = int(voltage.toDouble() * 1000);
            if (7000 <= integerVoltage && integerVoltage <= 16500) // the right one
            {
                this->voltage_mV = integerVoltage;
                ui.sliderVoltage->setSliderPosition(integerVoltage / 100);
                
                // TODO: output the voltage

                return;  // everything is OK, break in advance.
            }
            else
            {
                QMessageBox::information(
                    this,
                    "Range Mismatch",
                    "Input range must in [7.0, 16.5]",
                    QMessageBox::Yes);
            }
        }
        else
        {
            QMessageBox::information(
                this,
                "Type Error",
                "Only integer or float number is valid.",
                QMessageBox::Yes);
        }

        // input error occurs, reset the text
        ui.editVoltage->setText(QString::number(double(voltage_mV) / 1000, 'f', 1));
        ui.ckbPowerEnable->setChecked(false);
    }
}

// save default configuration
void setupFrame::saveDefaultConfig()
{
    this->saveConfig(Settings::Section::SETUP_DEFAULT);
}

// Uncheck output box when editing
void setupFrame::voltageEditing(QString text)
{
    ui.ckbPowerEnable->setChecked(false);
}

// Uncheck output box when editing
void setupFrame::PWM1_freqEditing(QString text)
{
    int value = ui.cbPWMfreqency1->currentText().toInt();
    if (value / 100 < 1  ||
        value / 100 > 5)         // 100~500
    {
        ui.ckbPWMEnable1->setChecked(false);
    }
}

// Uncheck output box when editing
void setupFrame::PWM2_freqEditing(QString text)
{
    int value = ui.cbPWMfreqency2->currentText().toInt();
    if (value / 100 < 1 ||
        value / 100 > 5)         // 100~500
    {
        ui.ckbPWMEnable2->setChecked(false);
    }
}

// output PWM1 signal
void setupFrame::PWM1_output(bool isChecked)
{
    if (isChecked)
    {
        // validator for PWM freq input(positive integer and 100<= x <=500 limited)
        QRegExp regx("^ [0-9]*[1-9][0-9]*$");
        auto freq = ui.cbPWMfreqency1->currentText();
        if (regx.exactMatch(freq))
        {
            int intFreq = freq.toInt();
            if (100 <= intFreq && intFreq <= 500) // the right one
            {
                // TODO: output PWM1 frequency `intFreq`

                return;  // everything is OK, break in advance.
            }
            else
            {
                QMessageBox::information(
                    this,
                    "Range Mismatch",
                    "Input range must in [100, 500]",
                    QMessageBox::Yes);
            }
        }
        else
        {
            QMessageBox::information(
                this,
                "Type Error",
                "Only positive integer number is valid.",
                QMessageBox::Yes);
        }

        // input error occurs, reset freq to lowest
        ui.cbPWMfreqency1->setCurrentIndex(0);
    }
}

// output PWM1 signal
void setupFrame::PWM2_output(bool isChecked)
{
    if (isChecked)
    {
        // validator for PWM freq input(positive integer and 100<= x <=500 limited)
        QRegExp regx("^ [0-9]*[1-9][0-9]*$");
        auto freq = ui.cbPWMfreqency2->currentText();
        if (regx.exactMatch(freq))
        {
            int intFreq = freq.toInt();
            if (100 <= intFreq && intFreq <= 500) // the right one
            {
                // TODO: output PWM2 frequency `intFreq`

                return;  // everything is OK, break in advance.
            }
            else
            {
                QMessageBox::information(
                    this,
                    "Range Mismatch",
                    "Input range must in [100, 500]",
                    QMessageBox::Yes);
            }
        }
        else
        {
            QMessageBox::information(
                this,
                "Type Error",
                "Only positive integer number is valid.",
                QMessageBox::Yes);
        }

        // input error occurs, reset freq to lowest
        ui.cbPWMfreqency2->setCurrentIndex(0);
    }
}

// Display name on 'cbLDFfilePath1'
void setupFrame::displayLDF_fileName1()
{
    QString fileName = this->getLDFfileName();
    if (!fileName.isEmpty())
    {
        ui.cbLDFfilePath1->insertItem(0, fileName);
        ui.cbLDFfilePath1->setCurrentIndex(0);
    }
}

// Display name on 'cbLDFfilePath2'
void setupFrame::displayLDF_fileName2()
{
    QString fileName = this->getLDFfileName();
    if (!fileName.isEmpty())
    {
        ui.cbLDFfilePath2->insertItem(0, fileName);
        ui.cbLDFfilePath2->setCurrentIndex(0);
    }
}

