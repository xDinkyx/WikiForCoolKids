#include "MainWindow.h"

#include "WikiPageLoader.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QTextBrowser>

#include <QFile>

#include <QDebug>

namespace
{
    const QString WIKI_FOLDER_LOCATION("C:/Users/Dimitri/Dropbox/ErdaWiki/");
    const QString WIKI_FILE_EXTENSION(".txt");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_html_view(nullptr)
{
    setObjectName("WikiMainWindow");
    setupGUI();

    showPage("Home");
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

    QPushButton* back_button = new QPushButton();
    back_button->setObjectName("BackButton");
    back_button->setEnabled(false);

    QPushButton* home_button = new QPushButton();
    home_button->setObjectName("HomeButton");

    QPushButton* next_button = new QPushButton();
    next_button->setObjectName("NextButton");

    QPushButton* edit_button = new QPushButton();
    edit_button->setObjectName("EditButton");

    QPushButton* settings_button = new QPushButton();
    settings_button->setObjectName("SettingsButton");

    QHBoxLayout* tool_bar_layout = new QHBoxLayout();
    tool_bar_layout->setContentsMargins(0, 0, 0, 0);
    tool_bar_layout->setSpacing(0);

    tool_bar_layout->addWidget(m_back_button, 0, Qt::AlignLeft);
    tool_bar_layout->addWidget(m_home_button, 0, Qt::AlignLeft);
    tool_bar_layout->addWidget(m_forward_button, 0, Qt::AlignLeft);
    tool_bar_layout->addWidget(edit_button, 0, Qt::AlignLeft);
    tool_bar_layout->addWidget(settings_button, 1, Qt::AlignLeft);

#ifdef _DEBUG
    // Button for testing style file changes
    QPushButton* reload_style_button = new QPushButton("QSS");
    connect(reload_style_button, &QPushButton::clicked, this, &MainWindow::loadStyle);
    tool_bar_layout->addWidget(reload_style_button, 0, Qt::AlignRight);
#endif

    QWidget* tool_bar_widget = new QWidget();
    tool_bar_widget->setObjectName("ToolBar");
    tool_bar_widget->setLayout(tool_bar_layout);

    return tool_bar_widget;
}

QWidget* MainWindow::createMainWidget()
{
    m_html_view = new QTextBrowser();

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_html_view);

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
    QString page_file_path = WIKI_FOLDER_LOCATION + pageName + WIKI_FILE_EXTENSION;

    QString page_html = WikiPageLoader::loadPageHTML(page_file_path);
    m_html_view->setHtml(page_html);
}
