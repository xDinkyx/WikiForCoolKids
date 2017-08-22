#include "MainWindow.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QFile>

#include <QDebug>

#include "../cpp-markdown/markdown.h"
#include <iostream>
#include <fstream>
#include <sstream>

namespace
{
    const QString WIKI_FOLDER_LOCATION("C:/Users/Dimitri/Dropbox/ErdaWiki/");
    const QString WIKI_FILE_EXTENSION(".txt");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("WikiMainWindow");
    setupGUI();

    loadPage("Home");
}

void MainWindow::setupGUI()
{
    setMinimumSize(900, 600);
    loadStyle();

    setMenuWidget(createToolBar());
    setCentralWidget(createMainWidget());
}

QWidget* MainWindow::createToolBar()
{
    QHBoxLayout* tool_bar_layout = new QHBoxLayout();
    tool_bar_layout->setContentsMargins(0, 0, 0, 0);
    tool_bar_layout->setSpacing(0);

    QPushButton* back_button = new QPushButton();
    back_button->setObjectName("BackButton");
    back_button->setEnabled(false);
    tool_bar_layout->addWidget(back_button, 0, Qt::AlignLeft);

    QPushButton* home_button = new QPushButton();
    home_button->setObjectName("HomeButton");
    tool_bar_layout->addWidget(home_button, 0, Qt::AlignLeft);

    QPushButton* next_button = new QPushButton();
    next_button->setObjectName("NextButton");
    tool_bar_layout->addWidget(next_button, 0, Qt::AlignLeft);

    QPushButton* edit_button = new QPushButton();
    edit_button->setObjectName("EditButton");
    tool_bar_layout->addWidget(edit_button, 0, Qt::AlignLeft);

    QPushButton* settings_button = new QPushButton();
    settings_button->setObjectName("SettingsButton");
    tool_bar_layout->addWidget(settings_button, 1, Qt::AlignLeft);

#ifdef _DEBUG
    QPushButton* reload_style_button = new QPushButton("QSS");
    tool_bar_layout->addWidget(reload_style_button, 0, Qt::AlignRight);
    connect(reload_style_button, &QPushButton::clicked, this, &MainWindow::loadStyle);
#endif

    QWidget* tool_bar_widget = new QWidget();
    tool_bar_widget->setObjectName("ToolBar");
    tool_bar_widget->setLayout(tool_bar_layout);

    return tool_bar_widget;
}

QWidget* MainWindow::createMainWidget()
{
    QLabel* temp_label = new QLabel("Text goes here");

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->addWidget(temp_label);

    QWidget* main_widget = new QWidget();
    main_widget->setLayout(main_layout);

    return main_widget;
}

void MainWindow::loadStyle()
{
#if _DEBUG
    QString qss_location = "../WikiForCoolKids/Resources/wiki_styles.qss";
#else
    QString qss_location = ":/Styles/Resources/wiki_styles.qss";
#endif
    QFile style_file(qss_location);
    style_file.open(QFile::ReadOnly);
    setStyleSheet(style_file.readAll());
}

void MainWindow::showPage(const QString & pageName)
{
    QString page_file_name = WIKI_FOLDER_LOCATION + pageName + WIKI_FILE_EXTENSION;

    QString page_html = loadPage(page_file_name);
}

QString MainWindow::loadPage(const QString& pageName)
{
    std::string file_path = pageName.toStdString();

    std::ifstream input_file_stream;
    std::istream* input_stream = &std::cin;
    if (!file_path.empty())
    {
        qDebug() << "Opening page '" << pageName << "'...";
        input_file_stream.open(file_path.c_str());
        if (!input_file_stream)
        {
            qDebug() << "Error: Can't open page file.";
            return;
        }
        else
        {
            qDebug() << "Opened page file.";
            input_stream = &input_file_stream;
        }
    }

    markdown::Document markdown_document;
    markdown_document.read(*input_stream);

    std::stringstream output_string_stream;
    markdown_document.write(output_string_stream);
    return QString::fromStdString(output_string_stream.str());
}
