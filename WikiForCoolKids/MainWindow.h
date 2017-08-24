#pragma once

#include <QtWidgets/QMainWindow>

class QLayout;
class QTextBrowser;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = Q_NULLPTR);

    void showPage(const QString& pageName);

private:
    void setupGUI();
    QWidget* createToolBar();
    QWidget* createMainWidget();

    void loadStyle();


    QTextBrowser* m_html_view;
};
