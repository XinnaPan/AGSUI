#include "controlFrame.h"
#include "RunAGM.h"
#include <QString>
#include <QRegExp>
#include <QRegExpValidator>

controlFrame::controlFrame(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    ui.setupUi(this);
    parent_ptr = parent;

    //TODO: Load AGM file to decide en/disable AGM2
    //TODO: Load AGM file to fill Torque and Speed box
    //this->setEnableAGMx_widgets(2, false);

    //Load [CONTROL_CURRENT] configuration
    this->loadConfig(Settings::Section::CONTROL_CURRENT);

    this->setValidatorForPos();   // set text validator for pos edit      (0~100)
    this->setValidatorForTime();  // set text validator for time interval (integer)
    this->initAGM_thread();       // init for AGM thread

    // power status timer
    connect(&powerStatusTimer, SIGNAL(timeout()), this, SLOT(checkPowerStatus()));
    powerStatusTimer.start(1000);   // 1000 ms
}

controlFrame::~controlFrame()
{
    AGM1_thread.quit();
    AGM2_thread.quit();
    AGM1_thread.wait();
    AGM2_thread.wait();
}

// create and move the object to new thread
void controlFrame::initAGM_thread()
{
    auto AGM1 = new RunAGM(1);
    auto AGM2 = new RunAGM(2);
    AGM1->moveToThread(&AGM1_thread);
    AGM2->moveToThread(&AGM2_thread);

    connect(&AGM1_thread, &QThread::finished, AGM1, &QObject::deleteLater);
    connect(&AGM2_thread, &QThread::finished, AGM2, &QObject::deleteLater);
    //
    connect(this, SIGNAL(runAGM_once(int)), AGM1, SLOT(runOnce(int)));
    connect(this, SIGNAL(runAGM_once(int)), AGM2, SLOT(runOnce(int)));
    connect(this, SIGNAL(runAGM_periodically(int)), AGM1, SLOT(runPeriodically(int)));
    connect(this, SIGNAL(runAGM_periodically(int)), AGM2, SLOT(runPeriodically(int)));
    connect(this, SIGNAL(stopAGM_running(int)), AGM1, SLOT(stop(int)));
    connect(this, SIGNAL(stopAGM_running(int)), AGM2, SLOT(stop(int)));
    //
    connect(AGM1, SIGNAL(enableGroupBox(int, bool)), this, SLOT(setEnableAGMx_widgets(int, bool)));
    connect(AGM2, SIGNAL(enableGroupBox(int, bool)), this, SLOT(setEnableAGMx_widgets(int, bool)));

    AGM1_thread.start();
    AGM2_thread.start();
}

void controlFrame::setImgForPowerStatus(PowerType type, bool lightUp)
{
    const QString imgs[]{ 
        QString(":/controlFrame/dark_green.png"),
        QString(":/controlFrame/light_green.png"),
        QString(":/controlFrame/dark_red.png"),
        QString(":/controlFrame/light_red.png") };
    QLabel* const labels[]{
        ui.labelPowerStatus,
        ui.labelCurrentStatus,
        ui.labelDcAdpStatus,
        ui.label9V_Status };
    
    int imgIndex = 0;
    int labelIndex = (int)type;
    
    switch (type)
    {
    case PowerType::DUT_POWER:
    case PowerType::EXT_DC_ADP:
    case PowerType::TYPE_C_9V:
        if (lightUp)
            imgIndex = 1;
        else
            imgIndex = 0;
        break;
    case PowerType::OVER_CURRENT:
        if (lightUp)
            imgIndex = 3;
        else
            imgIndex = 2;
        break;
    default:
        break;
    }
    
    labels[labelIndex]->setPixmap(QPixmap(imgs[imgIndex]));
}

// AGMx can be 1 or 2. 
void controlFrame::setEnableAGMx_widgets(int AGMx, bool isEnable)
{
    QWidget* const box[]{ nullptr, ui.groupBox1_1, ui.groupBox2_1 };
    box[AGMx]->setEnabled(isEnable);
}

void controlFrame::checkPowerStatus()
{
    // whether lightUp. Default is none is lighted up.
    static bool oldStatus[4]{ false, false, false, false };

    //TODO: check all four power status
    //@If(status differ)
    //@    change oldStatus
    //@    change image (setImgForPowerStatus)
    //******Demo******
    for (auto& s : oldStatus)
        s = !s;
    for (short i = 0; i < 4; i++)
        this->setImgForPowerStatus(PowerType(i), oldStatus[i]);
}

// Set validator for pos edit
void controlFrame::setValidatorForPos()
{
    QRegExp regExp("^100|[1-9][0-9]|[0-9]$"); // (0~100)
    auto vld = new QRegExpValidator(regExp, this);
    ui.editPOS1_1->setValidator(vld);
    ui.editPOS1_2->setValidator(vld);
    ui.editPOS2_1->setValidator(vld);
    ui.editPOS2_2->setValidator(vld);
}

// Set validator for time interval
void controlFrame::setValidatorForTime()
{
    QRegExp regExp("^\\d+$");  // integer
    auto vld = new QRegExpValidator(regExp, this);
    ui.editTime1->setValidator(vld);
    ui.editTime2->setValidator(vld);
}

// load current/default configuration ini
void controlFrame::loadConfig(Settings::Section section)
{
    Settings setter(section);

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.
    
    ui.sliderPOS1_1->setValue(
        setter.getValue(Settings::Keys::AGM1_POS1, 20).toInt());
    ui.sliderPOS1_2->setValue(
        setter.getValue(Settings::Keys::AGM1_POS2, 50).toInt());
    ui.sliderPOS2_1->setValue(
        setter.getValue(Settings::Keys::AGM2_POS1, 20).toInt());
    ui.sliderPOS2_2->setValue(
        setter.getValue(Settings::Keys::AGM2_POS2, 50).toInt());

    ui.ckbCalib1->setChecked(
        setter.getValue(Settings::Keys::AGM1_CALIB_EN, 0).toInt());
    ui.ckbCalib2->setChecked(
        setter.getValue(Settings::Keys::AGM2_CALIB_EN, 0).toInt());

    ui.ckbAutoCalib1->setChecked(
        setter.getValue(Settings::Keys::AGM1_AUTO_CALIB, 0).toInt());
    ui.ckbAutoCalib2->setChecked(
        setter.getValue(Settings::Keys::AGM2_AUTO_CALIB, 0).toInt());

    ui.ckbAutoRun1->setChecked(
        setter.getValue(Settings::Keys::AGM1_AUTORUN, 0).toInt());
    ui.ckbAutoRun2->setChecked(
        setter.getValue(Settings::Keys::AGM2_AUTORUN, 0).toInt());

    // TODO: Deal with Torque and Speed setting
    //@ int QComboBox::findText(QString)
}

// save current/default configuration ini
void controlFrame::saveConfig(Settings::Section section)
{
    Settings setter(section);

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    setter.setValue(Settings::Keys::AGM1_POS1, ui.sliderPOS1_1->value());
    setter.setValue(Settings::Keys::AGM1_POS2, ui.sliderPOS1_2->value());
    setter.setValue(Settings::Keys::AGM2_POS1, ui.sliderPOS2_1->value());
    setter.setValue(Settings::Keys::AGM2_POS2, ui.sliderPOS2_2->value());

    setter.setValue(
        Settings::Keys::AGM1_CALIB_EN, (int)ui.ckbCalib1->isChecked());
    setter.setValue(
        Settings::Keys::AGM2_CALIB_EN, (int)ui.ckbCalib2->isChecked());

    setter.setValue(
        Settings::Keys::AGM1_AUTO_CALIB, (int)ui.ckbAutoCalib1->isChecked());
    setter.setValue(
        Settings::Keys::AGM2_AUTO_CALIB, (int)ui.ckbAutoCalib2->isChecked());

    setter.setValue(
        Settings::Keys::AGM1_AUTORUN, (int)ui.ckbAutoRun1->isChecked());
    setter.setValue(
        Settings::Keys::AGM2_AUTORUN, (int)ui.ckbAutoRun2->isChecked());

    setter.setValue(
        Settings::Keys::AGM1_TORQUE, ui.cbTorque1->currentText());
    setter.setValue(
        Settings::Keys::AGM2_TORQUE, ui.cbTorque2->currentText());

    setter.setValue(
        Settings::Keys::AGM1_SPEED, ui.cbSpeed1->currentText());
    setter.setValue(
        Settings::Keys::AGM2_SPEED, ui.cbSpeed2->currentText());
}

void controlFrame::loadDefaultConfig()
{
    this->loadConfig(Settings::Section::CONTROL_DEFAULT);
}

void controlFrame::saveDefaultConfig()
{
    this->saveConfig(Settings::Section::CONTROL_DEFAULT);
}

// AGM1 pos1
void controlFrame::sliderPos1_1_changed(int value)
{
    const int pos2 = ui.sliderPOS1_2->value();
    if (value > pos2)
        ui.sliderPOS1_2->setValue(value);
    ui.editPOS1_1->setText(QString::number(value));
}

// AGM1 pos2
void controlFrame::sliderPos1_2_changed(int value)
{
    const int pos1 = ui.sliderPOS1_1->value();
    if (value < pos1)
        ui.sliderPOS1_1->setValue(value);
    ui.editPOS1_2->setText(QString::number(value));
}

// AGM2 pos1
void controlFrame::sliderPos2_1_changed(int value)
{
    const int pos2 = ui.sliderPOS2_2->value();
    if (value > pos2)
        ui.sliderPOS2_2->setValue(value);
    ui.editPOS2_1->setText(QString::number(value));
}

// AGM2 pos2
void controlFrame::sliderPos2_2_changed(int value)
{
    const int pos1 = ui.sliderPOS2_1->value();
    if (value < pos1)
        ui.sliderPOS2_1->setValue(value);
    ui.editPOS2_2->setText(QString::number(value));
}

// change the pos slider when editing line box
void controlFrame::posEditing(const QString& text)
{
    auto p = (QLineEdit*)sender();
    if (p == ui.editPOS1_1)
        ui.sliderPOS1_1->setValue(text.toInt());
    else if (p == ui.editPOS1_2)
        ui.sliderPOS1_2->setValue(text.toInt());
    else if (p == ui.editPOS2_1)
        ui.sliderPOS2_1->setValue(text.toInt());
    else
        ui.sliderPOS2_2->setValue(text.toInt());
}

// change btnRun's text
void controlFrame::changeRunText(bool isCheck)
{
    auto p = (QCheckBox*)sender();
    QPushButton* dest;

    if (p == ui.ckbAutoRun1)
        dest = ui.btnRun1;
    else
        dest = ui.btnRun2;

    if (isCheck)
        dest->setText("Start");
    else
        dest->setText("Run");
}

// button Run is clicked
void controlFrame::btnRunClicked()
{
    auto p = (QPushButton*)sender();
    QCheckBox* ckb;
    bool isReapeat;
    bool* isRunning;
    int AGMx;           // for disable function

    // do some assignment
    if (p == ui.btnRun1)
    {
        ckb = ui.ckbAutoRun1;
        isReapeat = ckb->isChecked();
        isRunning = &this->isAGM1running;
        AGMx = 1;
    }
    else
    {
        ckb = ui.ckbAutoRun2;
        isReapeat = ckb->isChecked();
        isRunning = &this->isAGM2running;
        AGMx = 2;
    }

    // 3 different situation
    if (!isReapeat)
    {
        //tell thread to run AGM once
        emit runAGM_once(AGMx);

        //Disable all control widget
        this->setEnableAGMx_widgets(AGMx, false);
    }
    else if (*isRunning)
    {
        // Let AGM stop
        emit stopAGM_running(AGMx);

        p->setText("Start");
        ckb->setEnabled(true);
        *isRunning = false;
    }
    else
    {
        //Let AGM run automatically
        emit runAGM_periodically(AGMx);

        p->setText("Stop");
        ckb->setEnabled(false);
        *isRunning = true;
    }

}

// override the virtual function.
// Save config and show mainFrame again
void controlFrame::closeEvent(QCloseEvent* event)
{
    //save current config
    this->saveConfig(Settings::Section::CONTROL_CURRENT);
    
    event->accept();
}
