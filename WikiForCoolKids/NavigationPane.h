#pragma once

#include <QFrame>

#include <memory>

class WikiHeaderListModel;
class QTreeView;

class NavigationPane : public QFrame
{
    Q_OBJECT

public:
    NavigationPane(QWidget* parent = nullptr);
    ~NavigationPane();

    void updateHeaders(const QString& htmlString);

    void setCurrentHeaderFromAnchor(const QString& anchor);

signals:
    void headerNavigated(QString anchor);

private:
    void createGUI();

    void headerSelected(const QModelIndex &index);

    QTreeView* m_tree_view;
    std::unique_ptr<WikiHeaderListModel> m_model;
};
