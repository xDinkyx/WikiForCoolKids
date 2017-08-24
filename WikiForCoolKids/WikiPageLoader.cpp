#include "WikiPageLoader.h"

#include "../cpp-markdown/markdown.h"
#include <iostream>
#include <fstream>
#include <sstream>

#include <QFileInfo>

#include <QDebug>

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
    return QString::fromStdString(output_string_stream.str());
}
