#include "SearchBox.h"

#include "../WFCKLib/WikiSearch.h"

#include <QLineEdit>
#include <QHBoxLayout>
#include <QPushButton>

#include <QEvent>
#include <QResizeEvent>

#include <QDebug>

namespace
{
    const QString INITIAL_TEXT = "Search for...";
}

SearchBox::SearchBox(QWidget* parent /*= nullptr*/)
    : QWidget(parent)
    , m_search_input(nullptr)
{
    setFixedWidth(250);
    setupGui();

    m_search_input->installEventFilter(this);
}

SearchBox::~SearchBox() = default;

void SearchBox::clear()
{
    m_search_input->clear();
}

void SearchBox::setupGui()
{
    m_search_input = new QLineEdit();
    m_search_input->setObjectName("SearchInput");
    m_search_input->setTextMargins(5, 5, 5, 5);
    setInitialText();

    QPushButton* search_button = new QPushButton();
    search_button->setObjectName("SearchButton");
    connect(search_button, &QPushButton::clicked, this, &SearchBox::search);

    QHBoxLayout* main_layout = new QHBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->setSpacing(0);
    main_layout->addWidget(m_search_input);
    main_layout->addWidget(search_button);

    this->setLayout(main_layout);
}

void SearchBox::setInitialText()
{
    m_search_input->setText(INITIAL_TEXT);
}

void SearchBox::search()
{
    qDebug() << "Searching for: " << m_search_input->text();

    //WikiSearch::getWikiFileNames();
}

bool SearchBox::eventFilter(QObject* object, QEvent* event)
{
    if (object == m_search_input)
    {
        if (event->type() == QEvent::FocusIn)
            clear();
        else if (event->type() == QEvent::FocusOut)
            if(m_search_input->text().isEmpty())
                setInitialText();
    }

    return false;
}
