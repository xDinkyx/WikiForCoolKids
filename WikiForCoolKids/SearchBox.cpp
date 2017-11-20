#include "SearchBox.h"

#include "WikiSearch.h"

#include "defines.h"

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
    , m_searcher(new WikiSearcher)
{
    setFixedWidth(250);
    setupGui();

    m_search_input->installEventFilter(this);

    // Init search thread
    qRegisterMetaType<WikiSearchResult>("WikiSearchResult");

    m_searcher->moveToThread(&m_search_thread);

    connect(&m_search_thread, &QThread::finished, m_searcher.get(), &QObject::deleteLater);
    connect(this, &SearchBox::startSearch, m_searcher.get(), &WikiSearcher::startSearching);
    connect(m_searcher.get(), &WikiSearcher::resultsReady, this, &SearchBox::displaySearchResult);

    m_search_thread.start();
}

SearchBox::~SearchBox()
{
    m_search_thread.quit();
    m_search_thread.wait();
}

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
    connect(m_search_input, &QLineEdit::returnPressed, this, &SearchBox::search);

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

bool SearchBox::eventFilter(QObject* object, QEvent* event)
{
    if (object == m_search_input)
    {
        if (event->type() == QEvent::FocusIn && m_search_input->text() == INITIAL_TEXT)
            clear();
        else if (event->type() == QEvent::FocusOut)
            if(m_search_input->text().isEmpty())
                setInitialText();
    }

    return false;
}

void SearchBox::search()
{
    startSearch(m_search_input->text(), WikiSettings::WIKI_FOLDER_LOCATION);
}

void SearchBox::displaySearchResult(const QList<WikiSearchResult*> intermediateResults)
{
        qDebug() << "---Search result---";
        for(WikiSearchResult* result : intermediateResults)
            qDebug() << result->m_wiki_page << " " << result->m_index;
}
