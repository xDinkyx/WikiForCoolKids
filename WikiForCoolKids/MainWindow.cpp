#include "MainWindow.h"

#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QHBoxLayout>

#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>

#include <QFile>

#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    setObjectName("WikiMainWindow");
    setupGUI();
}

void MainWindow::setupGUI()
{
    setMinimumSize(600, 400);
    loadStyle();

    setMenuWidget(createToolBar());
    setCentralWidget(createMainWidget());
}

QWidget* MainWindow::createToolBar()
{
    QHBoxLayout* tool_bar_layout = new QHBoxLayout();
    tool_bar_layout->setContentsMargins(5, 5, 5, 5);

    QPushButton* settings_button = new QPushButton();
    settings_button->setObjectName("SettingsButton");
    settings_button->setFixedSize(34, 34);
    tool_bar_layout->addWidget(settings_button, 0, Qt::AlignLeft);

#ifdef _DEBUG
    QPushButton* reload_style_button = new QPushButton("QSS");
    reload_style_button->setFixedSize(30, 30);
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
