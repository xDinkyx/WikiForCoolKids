#pragma once

#include <QStringList>

#include <QThread>

class QTimer;

class WikiSearch
{
public:
    static QStringList getWikiPagesContaining(const QString& searchString, const QString& wikiFolder);
    static QStringList getWikiFileNames(const QString& folder);
};

struct WikiSearchResult
{    
    QString m_wiki_page;
    int m_index{0};
};
typedef QList<WikiSearchResult*> WikiSearchResultList;
Q_DECLARE_METATYPE(WikiSearchResultList)

class WikiSearcher : public QObject
{
    Q_OBJECT;

public:
    WikiSearcher(QObject* parent = nullptr);

    void startSearching(const QString& searchString, const QString& searchFolder);

signals:
    void resultsReady(const WikiSearchResultList& intermediateResults);

private:
    QTimer* m_timer;
};
