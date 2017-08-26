#include "WikiPageLoader.h"

#include "../cpp-markdown/markdown.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <QFileInfo>

#include <QDebug>

QString WikiPageLoader::loadPageHTML(const QString& filePath)
{
    QFileInfo file_info(filePath);
    if (!file_info.isFile())
    {
        qDebug() << "File does not exist: " << filePath;
        return "";
    }

    markdown::Document markdown_document;
    markdown_document.read(readWikiFileString(filePath));

    std::stringstream output_string_stream;
    markdown_document.write(output_string_stream);

    QString html_string = QString::fromStdString(output_string_stream.str());
    parseHtml(html_string);

    return html_string;
}

std::string WikiPageLoader::readWikiFileString(const QString& filePath)
{
    std::string file_path = filePath.toStdString();

    std::ifstream input_file_stream;
    input_file_stream.open(file_path.c_str());
    if (!input_file_stream)
    {
        qDebug() << "Failed to open file: " << filePath;
        return "";
    }
    else
    {
        return std::string((std::istreambuf_iterator<char>(input_file_stream)), std::istreambuf_iterator<char>());
    }
}

void WikiPageLoader::parseHtml(QString& htmlString)
{
    QStringList html_lines = htmlString.split('\n');
    for (int i(0); i < html_lines.size(); ++i)
    {
         insertHtmlLinks(html_lines[i]);
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
            QString link = htmlLine.mid(link_start, link_end - link_start + 1);
            QString link_without_braces = link.mid(1, link.size() - 2);
            htmlLine.replace(link, QString("<a href=\"%1\">%2</a>").arg(link_without_braces.replace(' ', '_'), link_without_braces));

            link_start = htmlLine.indexOf('{');
        }
    }
}
