#pragma once

#include <QString>
#include <string>

class WikiPageLoader
{
public:
    static QString loadPageHTML(const QString& filePath);

private:
    static std::string readWikiFileString(const QString& filePath);

    static void parseHtml(QString& htmlString);
    static void insertHtmlLinks(QString& htmlLine);
};

