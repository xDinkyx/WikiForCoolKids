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

    void showPage(const QString& pageName);

private:

    // GUI
    void setupGUI();
    void loadStyle();
    QWidget* createToolBar();
    QWidget* createMainWidget();

    void updateButtonsEnabled();

    // Navigation
    void goToHomePage();
    void goToNextPage();
    void goToPreviousPage();
    void urlChanged(const QUrl& url);

    // Datamembers
    QPushButton* m_back_button;
    QPushButton* m_home_button;
    QPushButton* m_forward_button;

    QString m_current_page;
    QTextBrowser* m_html_view;
};
