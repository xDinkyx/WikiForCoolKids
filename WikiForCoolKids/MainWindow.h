#pragma once

#include <QtWidgets/QMainWindow>
#include <QUrl>

#include <list>

// Forward declarations
class QLayout;
class QPushButton;
class QStackedWidget;
class QTextBrowser;
class WikiEditView;

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
    void setupGUI();
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

    void finishEdit();
    void savePage();

    // Datamembers
    QPushButton* m_back_button;
    QPushButton* m_home_button;
    QPushButton* m_forward_button;
    QPushButton* m_edit_button;

    QStackedWidget* m_page_view;

    WikiPageList m_visited_pages;
    WikiPageList::iterator m_current_page;
    QTextBrowser* m_html_view;

    WikiEditView* m_edit_view;
};
