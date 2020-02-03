#include <fstream>
#include "Settings.h"


using namespace std;

const std::vector<QString> Settings::keys_qs{
    // Setup frame
    QString("DOWNLOAD_PORT"),
    QString("POWER_EN"),
    QString("POWER_VOLT"),
    QString("LIN_CH1_EN"),
    QString("LIN_CH1_BAUD"),
    QString("CH1_LDF_FILE_HISTORY1"),
    QString("CH1_LDF_FILE_HISTORY2"),
    QString("CH1_LDF_FILE_HISTORY3"),
    QString("CH1_LDF_FILE_HISTORY4"),
    QString("CH1_LDF_FILE_HISTORY5"),
    QString("CH1_LDF_FILE_HISTORY6"),
    QString("CH1_LDF_FILE_HISTORY7"),
    QString("CH1_LDF_FILE_HISTORY8"),
    QString("CH1_LDF_FILE_HISTORY9"),
    QString("CH1_LDF_FILE_HISTORY10"),
    QString("PWM_CH1_EN"),
    QString("PWM_CH1_FREQ"),
    QString("PIN5_FUNC_SEL"),
    QString("LIN_CH2_EN"),
    QString("LIN_CH2_BAUD"),
    QString("CH2_LDF_FILE_HISTORY1"),
    QString("CH2_LDF_FILE_HISTORY2"),
    QString("CH2_LDF_FILE_HISTORY3"),
    QString("CH2_LDF_FILE_HISTORY4"),
    QString("CH2_LDF_FILE_HISTORY5"),
    QString("CH2_LDF_FILE_HISTORY6"),
    QString("CH2_LDF_FILE_HISTORY7"),
    QString("CH2_LDF_FILE_HISTORY8"),
    QString("CH2_LDF_FILE_HISTORY9"),
    QString("CH2_LDF_FILE_HISTORY10"),
    QString("PWM_CH2_EN"),
    QString("PWM_CH2_FREQ"),

    // Program frame
    QString("FILE_PATH"),
    QString("DOWNLD_SEL_APP"),
    QString("DOWNLD_SEL_NVM"),
    QString("DOWNLD_SEL_PDATA"),
    QString("LAST_SERIAL"),

    // Control frame
    QString("AGM1_POS1"),
    QString("AGM1_POS2"),
    QString("AGM1_CALIB_EN"),
    QString("AGM1_AUTO_CALIB"),
    QString("AGM1_TORQUE"),
    QString("AGM1_SPEED"),
    QString("AGM1_AUTORUN"),
    QString("AGM2_POS1"),
    QString("AGM2_POS2"),
    QString("AGM2_CALIB_EN"),
    QString("AGM2_AUTO_CALIB"),
    QString("AGM2_TORQUE"),
    QString("AGM2_SPEED"),
    QString("AGM2_AUTORUN"),

    // Diagnosis frame
    QString("DIAG_SERVICE_SEL"),
    QString("SCRIPT_PATH"),

    // AutoTest frame
    QString("TEST_SCRIPT_PATH"),
    QString("ENABLE_SCRIPT_EDIT"),
};

const std::vector<QString> Settings::sections_qs{
    QString("SETUP_DEFAULT"),
    QString("SETUP_CURRENT"),
    QString("PROGRAM_DEFAULT"),
    QString("PROGRAM_CURRENT"),
    QString("CONTROL_DEFAULT"),
    QString("CONTROL_CURRENT"),
    QString("DIAGNOSIS_DEFAULT"),
    QString("DIAGNOSIS_CURRENT"),
    QString("AUTOTEST_DEFAULT"),
    QString("AUTOTEST_CURRENT"),
};

const QString Settings::filePath = QString(".\\x64\\agsuipara.ini");
const QString Settings::slash    = QString("/");


// construct function
Settings::Settings(Section section)
{
    this->section = section;
    this->config = new QSettings(filePath, QSettings::IniFormat);
}

Settings::~Settings()
{
    delete config;
}

// Set QVariant value for sepcified key.
void Settings::setValue(Keys key, const QVariant& value)
{    
    config->setValue(sections_qs[(int)section] + slash + keys_qs[(int)key],
                     value);
}

// Get value from specified key. If the key do not exist,
//default value will be returned.
QVariant Settings::getValue(Keys key, const QVariant& defaultValue)
{
    return config->value(sections_qs[(int)section] + slash + keys_qs[(int)key],
                         defaultValue);
}


// ---------- static function ----------------------
// Make sure at least an empty config file exists
void Settings::ensureConfig()
{
    ofstream out(filePath.toStdString(), ios::app);
    out.close();
}

