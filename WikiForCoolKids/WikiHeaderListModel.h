#pragma once

#include <QAbstractItemModel>

#include "WikiHeader.h"
#include <vector>
#include <memory>

class WikiHeaderListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    WikiHeaderListModel(QObject* parent = nullptr);
    ~WikiHeaderListModel();

    void setData(const std::vector<WikiHeader*>& headers);

    QVariant data(const QModelIndex& index, int role) const override;
    QModelIndex index(int row, int column, const QModelIndex& parent = QModelIndex()) const override;
    QModelIndex parent(const QModelIndex& child) const override;
    int rowCount(const QModelIndex& parent) const override;
    int columnCount(const QModelIndex& parent = QModelIndex()) const override;

private:
    std::unique_ptr<WikiHeader> m_top_header;
};
