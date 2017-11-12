#include "WikiSearch.h"

#include "WikiPageLoader.h"

#include "../WikiForCoolKids/defines.h"

#include <QDirIterator>

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

void WikiSearcher::startSearching(const QString& searchString, const QString& searchFolder)
{
    QStringList all_pages = WikiSearch::getWikiFileNames(searchFolder);

    // Search for string in page title.
    for (const QString& page : all_pages)
    {
        if (page.contains(searchString, Qt::CaseInsensitive))
        {
            WikiSearchResult result;
            result.m_wiki_page = page;
            emit resultReady(result);
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
                WikiSearchResult result;
                result.m_wiki_page = page;
                result.m_index = found_index;
                emit resultReady(result);

                found_index = page_markdown.indexOf(searchString, found_index + 1, Qt::CaseInsensitive);
            }
        }
        else
            qDebug() << "Error loading page markdown of " << page;
    }
}