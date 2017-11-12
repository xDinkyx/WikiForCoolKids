#include <QtCore/QCoreApplication>

#include "../WFCKLib/WikiCryptor.h"
#include "../WFCKLib/WikiSearch.h"

#include <QDirIterator>
#include <QFile>
#include <QTextStream>

const QString WIKI_FOLDER_LOCATION("C:/Users/Dimitri/Documents/ErdaWiki_Test/");

void print(const QString msg)
{
    static QTextStream ts(stdout);
    ts << msg << '\n';
    ts.flush();
}

void encryptDecryptFolder()
{
    print("-----------------------------");
    print("Encrypting/decrypting entire wiki folder.");
    print("---");

    QDirIterator it(WIKI_FOLDER_LOCATION, QStringList() << "*.txt", QDir::Files);
    while (it.hasNext())
    {
        QFile file(it.next());
        print(it.fileName());

        QString original_content;
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            original_content = in.readAll();
        }
        file.close();
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            out << WikiCryptor::encryptDecrypt(original_content);
        }
    }

    print("---");
    print("Encrypting/decrypting done.");
    print("-----------------------------");
}

void encryptThenDecryptFolder()
{
    print("-----------------------------");
    print("Encrypting and then decrypting entire wiki folder.");
    print("---");

    QDirIterator it(WIKI_FOLDER_LOCATION, QStringList() << "*.txt", QDir::Files);
    while (it.hasNext())
    {
        QFile file(it.next());
        print(it.fileName());

        QString original_content;
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            original_content = in.readAll();
        }
        file.close();
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            out << WikiCryptor::encryptDecrypt(original_content);
        }
        file.close();
        if (file.open(QIODevice::ReadOnly))
        {
            QTextStream in(&file);
            original_content = in.readAll();
        }
        file.close();
        if (file.open(QIODevice::WriteOnly))
        {
            QTextStream out(&file);
            out << WikiCryptor::encryptDecrypt(original_content);
        }
        file.close();
    }

    print("---");
    print("Encrypting and decrypting done.");
    print("-----------------------------");
}

//void fixErrorsInWikiFiles()
//{
//    print("-----------------------------");
//    print("Fixing known errors in wiki files.");
//    print("---");
//
//    QDirIterator it(WIKI_FOLDER_LOCATION, QStringList() << "*.txt", QDir::Files);
//    while (it.hasNext())
//    {
//        QFile file(it.next());
//        print(it.fileName());
//
//        QString original_content;
//        if (file.open(QIODevice::ReadOnly))
//        {
//            QTextStream stream(&file);
//            original_content = stream.readAll();
//        }
//        file.close();
//        if (file.open(QIODevice::WriteOnly))
//        {
//            QTextStream stream(&file);
//            stream << original_content.replace("’", "\'");
//        }
//    }
//
//    print("---");
//    print("Fixing issues done.");
//    print("-----------------------------");
//}

//void generateSearchFile()
//{
//    print("-----------------------------");
//    print("Generating search file.");
//    print("---");
//
//    auto headers = WikiSearch::getWikiFileNames(WIKI_FOLDER_LOCATION);
//    print(headers);
//
//    print("---");
//    print("Search file generated.");
//    print("-----------------------------");
//}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    QTextStream cin(stdin);

    print("Tool options");
    print("1 - Encrypt/decrypt wiki folder.");
    print("2 - Encrypt and then decrypt wiki folder.");
    //print("3 - Fix all known issues in wiki files.");
    //print("4 - Generate indexed search file.");

    QString input = cin.readLine();

    if (input == "1")
        encryptDecryptFolder();
    else if (input == "2")
        encryptThenDecryptFolder();
    //else if (input == "3")
    //    fixErrorsInWikiFiles();
    //else if (input == "4")
    //    generateSearchFile();

    return a.exec();
}
