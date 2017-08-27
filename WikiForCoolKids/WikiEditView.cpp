#include "WikiEditView.h"

#include <QVBoxLayout>
#include <QTextEdit>
#include <QPushButton>

WikiEditView::WikiEditView(QWidget *parent /*= nullptr*/)
    : QWidget(parent)
    , m_page_edit(nullptr)
{
    setObjectName("WikiEditView");
    setupGUI();
}

WikiEditView::~WikiEditView() = default;

void WikiEditView::setText(const QString& text)
{
    m_page_edit->setText(text);
}

QString WikiEditView::getText()
{
    return m_page_edit->toPlainText();
}

void WikiEditView::setupGUI()
{
    m_page_edit = new QTextEdit();

    QPushButton* save_button = new QPushButton("Close");
    connect(save_button, &QPushButton::clicked, this, &WikiEditView::editingFinished);

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(m_page_edit, 1);
    main_layout->addWidget(save_button);

    setLayout(main_layout);
}
