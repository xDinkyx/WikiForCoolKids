#include "WikiPageLoader.h"

#include "../cpp-markdown/markdown.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <QFileInfo>

#include "WikiHeader.h"

#include <QDebug>

namespace
{
    int extractHeaderLevel(const QString& headerString)
    {
        int level_pos = QString("<h").size();
        return headerString[level_pos].unicode() - '0';
    }

    QString extractHtmlElementContent(const QString& htmlElement)
    {
        int content_start = htmlElement.indexOf('>', htmlElement.indexOf("<a")) + 1;
        int content_length = htmlElement.length() - QString("</h0>").length() - content_start;
        return htmlElement.mid(content_start, content_length);
    }
}

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
    int line_number(0);
    for (QString& line : html_lines)
    {
         insertHtmlLinks(line);
         insertHtmlAnchors(line, line_number);
         ++line_number;
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

void WikiPageLoader::insertHtmlAnchors(QString& htmlLine, int lineNumber)
{
    if (htmlLine.startsWith("<h"))
    {
        int index = htmlLine.indexOf('>') + 1;
        htmlLine.insert(index, QString("<a name=\"#headerAnchor_%1\" />").arg(lineNumber));
    }
}

std::vector<WikiHeader*> WikiPageLoader::extractHeadersFromHtml(const QString& htmlString)
{
    std::vector<WikiHeader*> headers;
    WikiHeader* last_header;

    QStringList html_lines = htmlString.split('\n');
    for (const QString& line : html_lines)
    {
        if (line.startsWith("<h"))
        {
            int level = extractHeaderLevel(line);
            QString anchor = extractHtmlNameTag(line);
            QString name = extractHtmlElementContent(line);
            if (level == 1)
            {
                last_header = new WikiHeader(anchor, name, nullptr);
                headers.push_back(last_header);
            }
            else
            {
                int last_level = last_header->level();
                if (level > last_level)
                {
                    last_header = new WikiHeader(anchor, name, last_header);
                }
                else if (level == last_level)
                {
                    last_header = new WikiHeader(anchor, name, last_header->getParent());
                }
                else
                {
                    WikiHeader* parent_header = last_header->getParent();
                    while (parent_header->level() >= level)
                        parent_header = parent_header->getParent();
                    last_header = new WikiHeader(anchor, name, parent_header);
                }
            }
        }
    }

    return headers;
}

QStringList WikiPageLoader::removeEmptyHtmlLines(const QString & htmlString)
{
    auto html_lines = htmlString.split('\n');
    html_lines.removeAll("");
    html_lines.removeAll("<ul>");
    html_lines.removeAll("</ul>");
    return html_lines;
}

QString WikiPageLoader::extractHtmlNameTag(const QString & htmlElement)
{
    QString tag = "name";
    int name_start = htmlElement.indexOf(tag) + tag.length() + 2;
    int name_length = htmlElement.indexOf('"', name_start) - name_start;
    return htmlElement.mid(name_start, name_length);
}
