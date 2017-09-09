#include "WikiPageLoader.h"

#include "../cpp-markdown/markdown.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <QFileInfo>

#include "WikiHeader.h"

#include <QDebug>

bool WikiPageLoader::loadPage(const QString& filePath, QString& outMarkdown, QString& outHtml)
{
    QFileInfo file_info(filePath);
    if (!file_info.isFile())
    {
        qDebug() << "File does not exist: " << filePath;
        return "";
    }

    QString wiki_file_string;
    readWikiFile(filePath, wiki_file_string);

    parseWikiMarkdown(wiki_file_string, outMarkdown);

    markdown::Document markdown_document;
    markdown_document.read(outMarkdown.toStdString());

    std::stringstream output_string_stream;
    markdown_document.write(output_string_stream);

    outHtml = QString::fromStdString(output_string_stream.str());
    formatWikiHtml(outHtml);

    return true;
}

bool WikiPageLoader::parseWikiMarkdown(const QString& fileString, QString& out_markdown)
{
    out_markdown = fileString; // Currently entire file is markdown.

    return true;
}

bool WikiPageLoader::readWikiFile(const QString& filePath, QString& fileString)
{
    QFile file(filePath);
    if (file.open(QIODevice::ReadOnly))
    {
        QTextStream in(&file);
        fileString = in.readAll();
        file.close();

        return true;
    }
    return false;
}

void WikiPageLoader::formatWikiHtml(QString& htmlString)
{
    QStringList html_lines = htmlString.split('\n');
    for (QString& line : html_lines)
    {
         insertHtmlLinks(line);
    }
    htmlString = html_lines.join('\n');
}

void WikiPageLoader::insertHtmlLinks(QString& htmlLine)
{
    int link_start = htmlLine.indexOf('{');
    int link_end(0);

    while (link_start != -1)
    {
        link_end = htmlLine.indexOf('}');
        if (link_end != -1)
        {
            QString markdown_link = htmlLine.mid(link_start, link_end - link_start + 1);
            QString displayed_link = markdown_link.mid(1, markdown_link.size() - 2);
            QString html_link(displayed_link);
            html_link.replace(' ', '_');
            htmlLine.replace(markdown_link, QString("<a href=\"%1\">%2</a>").arg(html_link, displayed_link));

            link_start = htmlLine.indexOf('{');
        }
    }
}

std::vector<WikiHeader*> WikiPageLoader::extractHeadersFromHtml(QString & htmlString)
{
    std::vector<WikiHeader*> headers;
    WikiHeader* last_header;

    QStringList html_lines = htmlString.split('\n');
    for (QString& line : html_lines)
    {
        if (line.startsWith("<h"))
        {
            int level  = line[2].unicode() - '0';
            if (level == 1)
            {
                last_header = new WikiHeader("main", line, nullptr);
                headers.push_back(last_header);
            }
            else
            {
                int last_level = last_header->level();
                if (level > last_level)
                {
                    last_header = new WikiHeader("child", line, last_header);
                }
                else if (level == last_level)
                {
                    last_header = new WikiHeader("child", line, last_header->getParent());
                }
                else
                {
                    WikiHeader* parent_header = last_header->getParent();
                    while (parent_header->level() >= level)
                        parent_header = parent_header->getParent();
                    last_header = new WikiHeader("child", line, parent_header);
                }
            }
        }
    }

    return headers;
}
