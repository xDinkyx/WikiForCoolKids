#include "WikiHeaderListModel.h"

#include <QDebug>

WikiHeaderListModel::WikiHeaderListModel(QObject* parent /*= nullptr*/)
    : QAbstractItemModel(parent)
    , m_top_header(new WikiHeader("", "Navigation pane"))
{
}

WikiHeaderListModel::~WikiHeaderListModel() = default;

void WikiHeaderListModel::setData(const std::vector<WikiHeader*>& headers)
{
    for(WikiHeader* header : headers)
        m_top_header->addChild(header);
}

WikiHeader * WikiHeaderListModel::findHeaderByAnchor(const QString & anchor)
{
    return m_top_header->findHeaderByAnchor(anchor);
}

QVariant WikiHeaderListModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid())
        return QVariant();

    WikiHeader* header = static_cast<WikiHeader*>(index.internalPointer());
    switch (role)
    {
        case Qt::DisplayRole:
            return header->getName();
        case Qt::UserRole:
            return header->getAnchor();
        default:
            return QVariant();
    }
}

QModelIndex WikiHeaderListModel::index(int row, int column, const QModelIndex& parent /*= QModelIndex()*/) const
{
    if (!hasIndex(row, column, parent))
        return QModelIndex();

    WikiHeader* parent_header;
    if (!parent.isValid())
        parent_header = m_top_header.get();
    else
        parent_header = static_cast<WikiHeader*>(parent.internalPointer());

    WikiHeader* child_header = parent_header->childAt(row);
    if (child_header)
        return createIndex(row, column, child_header);
    else
        return QModelIndex();
}

QModelIndex WikiHeaderListModel::parent(const QModelIndex& child) const
{
    if (!child.isValid())
        return QModelIndex();

    WikiHeader* child_header = static_cast<WikiHeader*>(child.internalPointer());
    WikiHeader* parent_header = child_header->getParent();

    if (parent_header == m_top_header.get())
        return QModelIndex();

    return createIndex(parent_header->row(), 0, parent_header);
}

int WikiHeaderListModel::rowCount(const QModelIndex& parent) const
{
    WikiHeader *parent_header;
    if (parent.column() > 0)
        return 0;

    if (!parent.isValid())
        parent_header = m_top_header.get();
    else
        parent_header = static_cast<WikiHeader*>(parent.internalPointer());

    return parent_header->childCount();
}

int WikiHeaderListModel::columnCount(const QModelIndex& parent /*= QModelIndex()*/) const
{
    return 1;
}

