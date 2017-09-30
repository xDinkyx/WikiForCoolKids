#pragma once

#include <QtWidgets/QMainWindow>
#include <QUrl>

#include <list>

// Forward declarations
class QLayout;
class QPushButton;
class QStackedWidget;
class QTextEdit;
class QTextBrowser;
class NavigationPane;
class WikiEditView;
class SearchBox;

// Typedefs
typedef std::list<QString> WikiPageList;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void openWikiPage(const QString& pageName);

private:
    void setupGui();
    void loadStyle();
    void loadCSS();
    QWidget* createToolBar();
    QWidget* createMainWidget();
    QWidget* createHtmlView();
    QWidget* createPageEdit();

    void switchToEditView();
    void switchToHtmlView();
    void updateButtonsEnabled();

    void goToHomePage();
    void goToNextPage();
    void goToPreviousPage();
    void openLink(const QUrl& url);
    void clearNextPages();

    void scrollToHeader(QString headerAnchor);
    void updateCurrentHeader(int scrollPos);

    QString getVisibleText(QTextEdit* textEdit);

    void finishEdit(bool textChanged);
    void savePage();

    // Datamembers
    QPushButton* m_back_button;
    QPushButton* m_home_button;
    QPushButton* m_forward_button;
    QPushButton* m_edit_button;

    QStackedWidget* m_page_view;

    WikiPageList m_visited_pages;
    WikiPageList::iterator m_current_page;
    QTextBrowser* m_html_browser;
    QStringList m_pure_html_lines;  // page html with empty lines removed
    NavigationPane* m_navigation_pane;

    int m_current_line;
    QWidget* m_html_view;
    WikiEditView* m_edit_view;

    SearchBox* m_search_box;
};
