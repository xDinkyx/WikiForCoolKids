#pragma once

#include <QWidget>

class QLineEdit;

class SearchBox : public QWidget
{
    Q_OBJECT

public:
    SearchBox(QWidget* parent = nullptr);
    ~SearchBox();

    void clear();

private:
    void setupGui();
    void setInitialText();

    void search();

    bool eventFilter(QObject* object, QEvent* event) override;

    QLineEdit* m_search_input;
};
