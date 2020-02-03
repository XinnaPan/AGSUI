/* ***********************************************************
 * Class `DUTinfoValidator` to validator the information format
 *of device HW/SW/production date/serial number.
 * @Date: 2020/01/22
 * ***********************************************************/

#ifndef __DUTINFOVALIDATOR_H__
#define __DUTINFOVALIDATOR_H__

#include <QString>

class DUTinfoValidator
{
public:
	DUTinfoValidator();
	~DUTinfoValidator();
    static bool vldHWversion(const QString& hw);      // validate HW version
    static bool vldSWversion(const QString& oldSW,
                             const QString& newSW);   // validate production date
    static bool vldProductDate(const QString& date);  // validate SW version
    static bool vldSerial(const QString& serial);     // validate serial
};

#endif
