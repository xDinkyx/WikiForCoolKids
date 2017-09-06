#include "NavigationPane.h"

#include "WikiHeaderListModel.h"

#include <QFrame>
#include <QVBoxLayout>
#include <QTreeView>

NavigationPane::NavigationPane(QWidget* parent /*= nullptr*/)
    : QFrame(parent)
{
    setObjectName("NavigationPane");
    setMinimumSize(100, 0);
    createGUI();
}

NavigationPane::~NavigationPane() = default;

void NavigationPane::createGUI()
{
    QTreeView* tree_view = new QTreeView();
    WikiHeaderListModel* list_model = new WikiHeaderListModel();
    tree_view->setModel(list_model);

    QVBoxLayout* main_layout = new QVBoxLayout();
    main_layout->setContentsMargins(0, 0, 0, 0);
    main_layout->addWidget(tree_view);

    this->setLayout(main_layout);
}
