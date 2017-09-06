#include "WikiHeaderListModel.h"

WikiHeaderListModel::WikiHeaderListModel(QObject* parent /*= nullptr*/)
    : QAbstractListModel(parent)
{
    for (int i = 0; i < 100; i++)
    {
        m_wiki_headers.push_back(WikiHeader{"Path", QString("Header %1").arg(i), 0});
    }
}

WikiHeaderListModel::~WikiHeaderListModel() = default;

int WikiHeaderListModel::rowCount(const QModelIndex& parent) const
{
    return m_wiki_headers.size();
}

QVariant WikiHeaderListModel::data(const QModelIndex& index, int role) const
{
    WikiHeader header = m_wiki_headers[index.row()];
     
    switch (role)
    {
        case Qt::DisplayRole:
            return header.name;
        case Qt::DecorationRole:
        default:
            return QVariant();
    }
}

