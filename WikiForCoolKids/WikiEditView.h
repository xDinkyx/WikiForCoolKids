#pragma once

#include <QWidget>

class QTextEdit;
class QPushButton;

class WikiEditView : public QWidget
{
    Q_OBJECT

public:
    WikiEditView(QWidget *parent = nullptr);
    ~WikiEditView();

    void setText(const QString& text);
    QString getText();

signals:
    void editingFinished();

private:
    void setupGUI();

    QTextEdit* m_page_edit;
};
