#pragma once

#include <QtWidgets/QMainWindow>
#include <QUrl>

class QLayout;
class QPushButton;
class QTextBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

    void showWikiPage(const QString& pageName);

private:
    void setupGUI();
    void loadStyle();
    void loadCSS();
    QWidget* createToolBar();
    QWidget* createMainWidget();

    void updateButtonsEnabled();

    void goToHomePage();
    void goToNextPage();
    void goToPreviousPage();
    void openLink(const QUrl& url);

    // Datamembers
    QPushButton* m_back_button;
    QPushButton* m_home_button;
    QPushButton* m_forward_button;

    QString m_current_page;
    QTextBrowser* m_html_view;
};
