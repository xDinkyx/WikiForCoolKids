#pragma once

#include "wfcklib_global.h"

#include <QString>

class WFCKLIB_EXPORT WikiCryptor
{
public:
    static QString encryptDecrypt(const QString& toEncrypt);
};