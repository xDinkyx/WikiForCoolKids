#pragma once

#include <QAbstractListModel>

#include "WikiHeader.h"
#include <vector>

class WikiHeaderListModel : public QAbstractListModel
{
    Q_OBJECT

public:
    WikiHeaderListModel(QObject* parent = nullptr);
    ~WikiHeaderListModel();

    int rowCount(const QModelIndex& parent) const override;
    QVariant data(const QModelIndex& index, int role) const override;

private:
    std::vector<WikiHeader> m_wiki_headers;
};
