/* ***********************************************************
 * Class `Settings` to deal with reading and writing ini file.
 * @Date: 2020/01/18
 * ***********************************************************/

#ifndef __SETTINGS_H__
#define __SETTINGS_H__

#include <vector>
#include <QString>
#include <QSettings>
#include <QVariant>

class Settings
{
public:
    enum class Keys : int
    {
        // Setup frame
        DOWNLOAD_PORT,
        POWER_EN,
        POWER_VOLT,
        LIN_CH1_EN,
        LIN_CH1_BAUD,
        CH1_LDF_FILE_HISTORY1,
        CH1_LDF_FILE_HISTORY2,
        CH1_LDF_FILE_HISTORY3,
        CH1_LDF_FILE_HISTORY4,
        CH1_LDF_FILE_HISTORY5,
        CH1_LDF_FILE_HISTORY6,
        CH1_LDF_FILE_HISTORY7,
        CH1_LDF_FILE_HISTORY8,
        CH1_LDF_FILE_HISTORY9,
        CH1_LDF_FILE_HISTORY10,
        PWM_CH1_EN,
        PWM_CH1_FREQ,
        PIN5_FUNC_SEL,
        LIN_CH2_EN,
        LIN_CH2_BAUD,
        CH2_LDF_FILE_HISTORY1,
        CH2_LDF_FILE_HISTORY2,
        CH2_LDF_FILE_HISTORY3,
        CH2_LDF_FILE_HISTORY4,
        CH2_LDF_FILE_HISTORY5,
        CH2_LDF_FILE_HISTORY6,
        CH2_LDF_FILE_HISTORY7,
        CH2_LDF_FILE_HISTORY8,
        CH2_LDF_FILE_HISTORY9,
        CH2_LDF_FILE_HISTORY10,
        PWM_CH2_EN,
        PWM_CH2_FREQ,

        // Program frame
        FILE_PATH,
        DOWNLD_SEL_APP,
        DOWNLD_SEL_NVM,
        DOWNLD_SEL_PDATA,
        LAST_SERIAL,

        // Control frame
        AGM1_POS1,
        AGM1_POS2,
        AGM1_CALIB_EN,
        AGM1_AUTO_CALIB,
        AGM1_TORQUE,
        AGM1_SPEED,
        AGM1_AUTORUN,
        AGM2_POS1,
        AGM2_POS2,
        AGM2_CALIB_EN,
        AGM2_AUTO_CALIB,
        AGM2_TORQUE,
        AGM2_SPEED,
        AGM2_AUTORUN,

        // Diagnosis frame
        DIAG_SERVICE_SEL,
        SCRIPT_PATH,

        // AutoTest frame
        TEST_SCRIPT_PATH,
        ENABLE_SCRIPT_EDIT,
    };
    enum class Section : int
    {
        SETUP_DEFAULT,
        SETUP_CURRENT,
        PROGRAM_DEFAULT,
        PROGRAM_CURRENT,
        CONTROL_DEFAULT,
        CONTROL_CURRENT,
        DIAGNOSIS_DEFAULT,
        DIAGNOSIS_CURRENT,
        AUTOTEST_DEFAULT,
        AUTOTEST_CURRENT,
    };

    Settings(Section section);
    ~Settings();
    static void ensureConfig();   // make sure at least an empty config file exists
    void setValue(Keys key, const QVariant& value);           // set QVariant value
    QVariant getValue(Keys key, const QVariant& defaultValue);// get value

private:
    const static std::vector<QString> keys_qs;
    const static std::vector<QString> sections_qs;
    const static QString filePath;
    const static QString slash;
    //
    Section section;
    QSettings* config;
};

#endif
