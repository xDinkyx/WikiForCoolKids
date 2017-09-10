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

    void goToLine(int line);

signals:
    void editingFinished(bool textChanged);

private:
    void setupGUI();
    void updateFinishButtonText();
    void finishEditing();

    QTextEdit* m_page_edit;
    QPushButton* m_finish_button;

    QString m_original_text;
};
