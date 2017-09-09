#pragma once

#include <QString>
#include <string>

#include <vector>

class WikiHeader;

class WikiPageLoader
{
public:
    // Read file at path and fill in markdown and html string.
    static bool loadPage(const QString& filePath, QString& outMarkdown, QString& outHtml);
    // Take entire file string and extract the markdown string.
    static bool parseWikiMarkdown(const QString& fileString, QString& outMarkdown);

    // Takes html and extracts headers.
    static std::vector<WikiHeader*> extractHeadersFromHtml(const QString& htmlString);

private:
    // Open file and read in contents.
    static bool readWikiFile(const QString& filePath, QString& fileString);

    // Takes html string and adds in missing parts.
    static void formatWikiHtml(QString& htmlString);
    static void insertHtmlLinks(QString& htmlLine);
    static void insertHtmlAnchors(QString& htmlLine, int lineNumber);
};

