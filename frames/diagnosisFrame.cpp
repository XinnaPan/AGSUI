#include "diagnosisFrame.h"
#include <QFileDialog>

diagnosisFrame::diagnosisFrame(QWidget *parent)
    : QWidget(parent, Qt::Window)
{
    ui.setupUi(this);
    parent_ptr = parent;

    //TODO:Load Diagnosis service selection box "diag_service_def.dsf"

    //Load [DIAGNOSIS_CURRENT] configuration
    this->loadConfig(Settings::Section::DIAGNOSIS_CURRENT);

}

diagnosisFrame::~diagnosisFrame()
{
}

// load current/default configuration ini
void diagnosisFrame::loadConfig(Settings::Section section)
{
    Settings setter(section);
    using Keys = Settings::Keys;

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    auto txt = setter.getValue(Keys::DIAG_SERVICE_SEL, "").toString();
    int index = ui.cbServSelection->findText(txt);
    if (index != -1)
        ui.cbServSelection->setCurrentIndex(index);

    this->scriptPath = setter.getValue(Keys::SCRIPT_PATH, ".\\").toString();
}

// save current/default configuration ini
void diagnosisFrame::saveConfig(Settings::Section section)
{
    Settings setter(section);
    using Keys = Settings::Keys;

    // you can easily know which property is being processed by
    //looking at the `Settings::Keys` enum class.

    setter.setValue(
        Keys::DIAG_SERVICE_SEL, ui.cbServSelection->currentText());
    setter.setValue(Keys::SCRIPT_PATH, this->scriptPath);
}

void diagnosisFrame::loadDefaultConfig()
{
    this->loadConfig(Settings::Section::DIAGNOSIS_DEFAULT);
}

void diagnosisFrame::saveDefaultConfig()
{
    this->saveConfig(Settings::Section::DIAGNOSIS_DEFAULT);
}

// btnRunScript clicked
void diagnosisFrame::runScriptClick()
{
    // get script path
    auto path = QFileDialog::getOpenFileName(
        this,
        "Open script file...",
        this->scriptPath,
        "Script (*.bat);;All files (*.*)");
    if (!path.isEmpty())
    {
        this->scriptPath = path;

        //TODO: load script and do some other things
    }
}


// override the virtual function.
// Save config and show mainFrame again
void diagnosisFrame::closeEvent(QCloseEvent* event)
{
    //save current config
    this->saveConfig(Settings::Section::DIAGNOSIS_CURRENT);

    event->accept();
}
