#include "WikiSearch.h"

#include "../WFCKLib/WikiPageLoader.h"

#include "defines.h"

#include <QDirIterator>

#include <QTimer>

#include <QDebug>

QStringList WikiSearch::getWikiPagesContaining(const QString& searchString, const QString& wikiFolder)
{
    QStringList found_pages;

    for (const QString& page : getWikiFileNames(wikiFolder))
    {
        if (page.contains(searchString, Qt::CaseInsensitive))
        {
            found_pages.push_back(page);
        }
    }

    return found_pages;
}

QStringList WikiSearch::getWikiFileNames(const QString& folder)
{
    QStringList fileNames;

    QDirIterator it(folder, QStringList() << "*.txt", QDir::Files);
    while (it.hasNext())
    {
        QFileInfo file(it.next());

        fileNames.push_back(file.baseName());
    }

    return fileNames;
}

WikiSearcher::WikiSearcher(QObject* parent /*= nullptr*/)
    : QObject(parent)
    , m_timer(new QTimer(this))
{
}

void WikiSearcher::startSearching(const QString& searchString, const QString& searchFolder)
{
    QStringList all_pages = WikiSearch::getWikiFileNames(searchFolder);
    QList<WikiSearchResult*> search_result_list;

    m_timer->start(100);
    connect(m_timer, &QTimer::timeout, [this, &search_result_list]() 
    { 
        emit resultsReady(search_result_list); 
        search_result_list.clear();
    });

    // Search for string in page title.
    for (const QString& page : all_pages)
    {
        if (page.contains(searchString, Qt::CaseInsensitive))
        {
            WikiSearchResult* result = new WikiSearchResult();
            result->m_wiki_page = page;
            search_result_list.push_back(result);
        }
    }

    // Search page contents.
    QString page_markdown;
    for (const QString& page : all_pages)
    {
        QString page_file_path = WikiSettings::WIKI_FOLDER_LOCATION + page + WikiSettings::WIKI_FILE_EXTENSION;
        if (WikiPageLoader::loadPageMarkdown(page_file_path, page_markdown))
        {
            int found_index = page_markdown.indexOf(searchString, Qt::CaseInsensitive);
            while (found_index != -1)
            {
                WikiSearchResult* result = new WikiSearchResult();
                result->m_wiki_page = page;
                result->m_index = found_index;
                search_result_list.push_back(result);

                found_index = page_markdown.indexOf(searchString, found_index + 1, Qt::CaseInsensitive);
            }
        }
        else
            qDebug() << "Error loading page markdown of " << page;
    }

    m_timer->stop();
    emit resultsReady(search_result_list);
}