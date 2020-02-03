#include "DUTinfoValidator.h"
#include <QRegExp>

DUTinfoValidator::DUTinfoValidator()
{
}

DUTinfoValidator::~DUTinfoValidator()
{
}

bool DUTinfoValidator::vldHWversion(const QString& hw)
{
    QRegExp expr("^[0-9][A-Z][0-9][0-9]$");   //change for actual requirement!
    if (expr.exactMatch(hw))
        return true;
    else
        return false;
}

bool DUTinfoValidator::vldSWversion(
    const QString& oldSW, 
    const QString& newSW)
{
    QRegExp expr("^[0-9]{4,4}$");   //change for actual requirement!
    if (oldSW.isEmpty())
    {
        if (newSW.isEmpty())
            return true;
        else if (expr.exactMatch(newSW))
            return true;
        else
            return false;
    }
    else   // old not empty
    {
        if (newSW.isEmpty())
            return true;
        else if (expr.exactMatch(newSW) &&
                 newSW.toInt() >= oldSW.toInt())
            return true;
        else
            return false;
    }
}

bool DUTinfoValidator::vldProductDate(const QString& date)
{
    QRegExp expr("^[0-9]{6,6}$");   //change for actual requirement!
    if (expr.exactMatch(date))
        return true;
    else
        return false;
}

bool DUTinfoValidator::vldSerial(const QString& serial)
{
    QRegExp expr("^[0-9]{4,4}$");   //change for actual requirement!
    if (expr.exactMatch(serial))
        return true;
    else
        return false;
}
