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

    const QString HOME_PAGE("Home");
    const QString CSS_FILE_NAME("style.css");
}

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , m_back_button(nullptr)
    , m_home_button(nullptr)
    , m_forward_button(nullptr)
    , m_html_view(nullptr)
{
    setObjectName("WikiMainWindow");
    setupGUI();

    goToHomePage();
}

void MainWindow::showWikiPage(const QString & pageName)
{
    m_current_page = pageName;

    QString page_file_path = WIKI_FOLDER_LOCATION + pageName + WIKI_FILE_EXTENSION;
    QString page_html = WikiPageLoader::loadPageHTML(page_file_path);
    m_html_view->setHtml(page_html);

    updateButtonsEnabled();
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

    m_back_button = new QPushButton();
    m_back_button->setObjectName("BackButton");
    connect(m_back_button, &QPushButton::clicked, this, &MainWindow::goToNextPage);

    m_home_button = new QPushButton();
    m_home_button->setObjectName("HomeButton");
    connect(m_home_button, &QPushButton::clicked, this, &MainWindow::goToHomePage);

    m_forward_button = new QPushButton();
    m_forward_button->setObjectName("NextButton");
    connect(m_forward_button, &QPushButton::clicked, this, &MainWindow::goToPreviousPage);

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
    m_html_view->setOpenLinks(false);
    m_html_view->setOpenExternalLinks(false);
    m_html_view->setSearchPaths(QStringList() << WIKI_FOLDER_LOCATION);
    connect(m_html_view, &QTextBrowser::anchorClicked, this, &MainWindow::openLink);
    loadCSS();

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_html_view);

    QWidget* main_widget = new QWidget();
    main_widget->setLayout(main_layout);

    return main_widget;
}

void MainWindow::updateButtonsEnabled()
{
    m_back_button->setEnabled(false);
    m_home_button->setEnabled(m_current_page != HOME_PAGE);
    m_forward_button->setEnabled(false);
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

void MainWindow::goToHomePage()
{
    showWikiPage(HOME_PAGE);
}

void MainWindow::goToNextPage()
{

}

void MainWindow::goToPreviousPage()
{

}

void MainWindow::openLink(const QUrl& url)
{
    showWikiPage(url.toString());
}

void MainWindow::loadCSS()
{
    QString style_file_path = WIKI_FOLDER_LOCATION + CSS_FILE_NAME;

    QFile file(style_file_path);
    if (file.open(QIODevice::ReadOnly)) 
    {
        QTextStream in(&file);

        QString css_string;
        while (!in.atEnd()) 
            css_string += in.readLine();
        m_html_view->document()->setDefaultStyleSheet(css_string);

        file.close();
    }
}
