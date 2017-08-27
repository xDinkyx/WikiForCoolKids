#include "MainWindow.h"

#include "WikiPageLoader.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

#include "WikiEditView.h"
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStackedWidget>
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

MainWindow::MainWindow(QWidget *parent /*= nullptr*/)
    : QMainWindow(parent)
    , m_back_button(nullptr)
    , m_home_button(nullptr)
    , m_forward_button(nullptr)
    , m_edit_button(nullptr)
    , m_page_view(nullptr)
    , m_html_view(nullptr)
    , m_edit_view(nullptr)
{
    setObjectName("WikiMainWindow");
    setupGUI();

    goToHomePage();
}

MainWindow::~MainWindow() = default;

void MainWindow::openWikiPage(const QString & pageName)
{
    QString page_file_path = WIKI_FOLDER_LOCATION + pageName + WIKI_FILE_EXTENSION;
    QString page_markdown, page_html;
    WikiPageLoader::loadPage(page_file_path, page_markdown, page_html);
    m_edit_view->setText(page_markdown);
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
    connect(m_back_button, &QPushButton::clicked, this, &MainWindow::goToPreviousPage);

    m_home_button = new QPushButton();
    m_home_button->setObjectName("HomeButton");
    connect(m_home_button, &QPushButton::clicked, this, &MainWindow::goToHomePage);

    m_forward_button = new QPushButton();
    m_forward_button->setObjectName("NextButton");
    connect(m_forward_button, &QPushButton::clicked, this, &MainWindow::goToNextPage);

    m_edit_button = new QPushButton();
    m_edit_button->setObjectName("EditButton");
    connect(m_edit_button, &QPushButton::clicked, this, &MainWindow::switchToEditView);

    QPushButton* settings_button = new QPushButton();
    settings_button->setObjectName("SettingsButton");

    QHBoxLayout* tool_bar_layout = new QHBoxLayout();
    tool_bar_layout->setContentsMargins(0, 0, 0, 0);
    tool_bar_layout->setSpacing(0);

    tool_bar_layout->addWidget(m_back_button, 0, Qt::AlignLeft);
    tool_bar_layout->addWidget(m_home_button, 0, Qt::AlignLeft);
    tool_bar_layout->addWidget(m_forward_button, 0, Qt::AlignLeft);
    tool_bar_layout->addWidget(m_edit_button, 0, Qt::AlignLeft);
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
    m_page_view = new QStackedWidget();
    m_page_view->addWidget(createHtmlView());
    m_page_view->addWidget(createPageEdit());
    m_page_view->setCurrentWidget(m_html_view);

    return m_page_view;
}

QWidget* MainWindow::createHtmlView()
{
    m_html_view = new QTextBrowser();
    m_html_view->setOpenLinks(false);
    m_html_view->setOpenExternalLinks(false);
    m_html_view->setSearchPaths(QStringList() << WIKI_FOLDER_LOCATION);
    connect(m_html_view, &QTextBrowser::anchorClicked, this, &MainWindow::openLink);
    loadCSS();

    return m_html_view;
}

QWidget* MainWindow::createPageEdit()
{
    m_edit_view = new WikiEditView();
    connect(m_edit_view, &WikiEditView::editingFinished, this, &MainWindow::finishEdit);

    return m_edit_view;
}

void MainWindow::switchToEditView()
{
    m_page_view->setCurrentWidget(m_edit_view);
    updateButtonsEnabled();
}

void MainWindow::switchToHtmlView()
{
    m_page_view->setCurrentWidget(m_html_view);
    openWikiPage(*m_current_page);
}

void MainWindow::updateButtonsEnabled()
{
    m_back_button->setEnabled(m_current_page != m_visited_pages.begin());
    m_home_button->setEnabled(*m_current_page != HOME_PAGE);
    m_forward_button->setEnabled(m_current_page != --m_visited_pages.end());
    m_edit_button->setEnabled(m_page_view->currentWidget() != m_edit_view);
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
    openLink(HOME_PAGE);
}

void MainWindow::goToNextPage()
{
    ++m_current_page;
    openWikiPage(*m_current_page);
}

void MainWindow::goToPreviousPage()
{
    --m_current_page;
    openWikiPage(*m_current_page);
}

void MainWindow::openLink(const QUrl& url)
{
    clearNextPages();

    m_visited_pages.push_back(url.toString());
    m_current_page = --m_visited_pages.end(); // Current page is last added iterator.

    openWikiPage(url.toString());
}

void MainWindow::clearNextPages()
{
    if (m_visited_pages.empty())
        return;
    if (m_current_page == --m_visited_pages.end())
        return;

    for (auto it = std::next(m_current_page); it != m_visited_pages.end();)
    {
        m_visited_pages.erase(it++);
    }
}

void MainWindow::finishEdit(bool textChanged)
{
    if (textChanged)
        savePage();
    
    switchToHtmlView();
}

void MainWindow::savePage()
{
    QString page_file_path = WIKI_FOLDER_LOCATION + *m_current_page + WIKI_FILE_EXTENSION;

    QFile file(page_file_path);
    if (file.open(QIODevice::WriteOnly))
    {
        QTextStream stream(&file);
        stream << m_edit_view->getText();
    }
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
