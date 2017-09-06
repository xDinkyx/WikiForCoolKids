#pragma once

#include <QFrame>

class NavigationPane : public QFrame
{
    Q_OBJECT

public:
    NavigationPane(QWidget* parent = nullptr);
    ~NavigationPane();

private:
    void createGUI();
};
