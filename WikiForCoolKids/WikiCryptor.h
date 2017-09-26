#pragma once

#include <QString>

QString encryptDecrypt(QString toEncrypt) 
{
    char key[]{'w', 'f', 'c', 'k'}; //Any chars will work
    std::string original = toEncrypt.toStdString();
    std::string output = toEncrypt.toStdString();

    for (int i = 0; i < original.size(); i++)
        output[i] = original[i] ^ key[i % (sizeof(key) / sizeof(char))];

    return QString::fromStdString(output);
}