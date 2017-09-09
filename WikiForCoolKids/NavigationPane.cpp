#include "NavigationPane.h"

#include "WikiHeaderListModel.h"

#include "WikiPageLoader.h"

#include <QFrame>
#include <QVBoxLayout>
#include <QTreeView>

NavigationPane::NavigationPane(QWidget* parent /*= nullptr*/)
    : QFrame(parent)
    , m_tree_view(nullptr)
    , m_model(nullptr)
{
    setObjectName("NavigationPane");

    setMaximumWidth(250);
    createGUI();
}

NavigationPane::~NavigationPane() = default;

void NavigationPane::updateHeaders(const QString& htmlString)
{
    m_model.reset(new WikiHeaderListModel());
    m_model->setData(WikiPageLoader::extractHeadersFromHtml(htmlString));

    m_tree_view->setModel(m_model.get());
    m_tree_view->expandAll();
    m_tree_view->resizeColumnToContents(0);
}

void NavigationPane::createGUI()
{
    m_tree_view = new QTreeView();
    m_tree_view->setHeaderHidden(true);
    m_tree_view->setAlternatingRowColors(true);

    m_tree_view->setEditTriggers(QAbstractItemView::CurrentChanged);

    connect(m_tree_view, &QTreeView::clicked, this, &NavigationPane::headerSelected);

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(m_tree_view);

    this->setLayout(main_layout);
}

void NavigationPane::headerSelected(const QModelIndex& index)
{
    WikiHeader* selected_header = static_cast<WikiHeader*>(index.internalPointer());
    emit headerNavigated(selected_header->getAnchor());
}
