#include "WikiSearch.h"

QStringList WikiSearch::getWikiFileNames(const QString & folder)
{
    QStringList headers;

    QDirIterator it(folder, QStringList() << "*.txt", QDir::Files);
    while (it.hasNext())
    {
        QFileInfo file(it.next());

        headers.push_back(file.baseName());
    }

    return headers;
}
