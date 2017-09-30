#pragma once

#include <QString>

namespace WikiCryptor
{
    static QString encryptDecrypt(const QString& toEncrypt)
    {
        wchar_t key[]{'w', 'f', 'c', 'k'}; //Any chars will work
        std::wstring original = toEncrypt.toStdWString();
        std::wstring output;
        output.resize(original.size());

        for (int i = 0; i < original.size(); i++)
            output[i] = original[i] ^ key[i % (sizeof(key) / sizeof(wchar_t))];

        return QString::fromStdWString(output);
    }
};