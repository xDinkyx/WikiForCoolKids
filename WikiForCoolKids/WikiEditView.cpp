#include "WikiEditView.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>
#include <QTextBlock>

WikiEditView::WikiEditView(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , m_page_edit(nullptr)
    , m_finish_button(nullptr)
{
    setObjectName("WikiEditView");
    setupGui();
}

WikiEditView::~WikiEditView() = default;

void WikiEditView::setText(const QString& text)
{
    m_original_text = text;
    m_page_edit->setText(text);
}

QString WikiEditView::getText()
{
    return m_page_edit->toPlainText();
}

void WikiEditView::goToLine(int line)
{
    QTextCursor cursor(m_page_edit->document()->findBlockByLineNumber(line)); // ln-1 because line number starts from 0
    m_page_edit->setTextCursor(cursor);
}

void WikiEditView::setupGui()
{
    m_page_edit = new QTextEdit();
    connect(m_page_edit, &QTextEdit::textChanged, this, &WikiEditView::updateFinishButtonText);

    m_finish_button = new QPushButton("Close");
    m_finish_button->setFixedHeight(30);
    connect(m_finish_button, &QPushButton::clicked, this, &WikiEditView::finishEditing);

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(m_page_edit);
    main_layout->addWidget(m_finish_button);

    setLayout(main_layout);
}

void WikiEditView::updateFinishButtonText()
{
    if (m_original_text != m_page_edit->toPlainText())
        m_finish_button->setText("Save");
    else
        m_finish_button->setText("Close");
}

void WikiEditView::finishEditing()
{
    emit editingFinished(m_original_text != m_page_edit->toPlainText());
}
