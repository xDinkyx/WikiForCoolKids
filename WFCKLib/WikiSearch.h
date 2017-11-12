#pragma once

#include "wfcklib_global.h"

#include <QStringList>
#include <QDirIterator>

class WFCKLIB_EXPORT WikiSearch
{
public:
    static QStringList getWikiFileNames(const QString& folder);
};

