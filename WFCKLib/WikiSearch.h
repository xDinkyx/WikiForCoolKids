#pragma once

#include "wfcklib_global.h"

#include <QStringList>

#include <QThread>

class WFCKLIB_EXPORT WikiSearch
{
public:
    static QStringList getWikiPagesContaining(const QString& searchString, const QString& wikiFolder);
    static QStringList getWikiFileNames(const QString& folder);
};

struct WikiSearchResult 
{    QString m_wiki_page;
    int m_index{0};
};
Q_DECLARE_METATYPE(WikiSearchResult)

class WikiSearcher : public QObject
{
    Q_OBJECT;

public:
    void startSearching(const QString& searchString, const QString& searchFolder);

signals:
    void resultReady(const WikiSearchResult& result);
};
