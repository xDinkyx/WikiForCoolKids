#pragma once

#include <QFrame>

class WikiHeaderListModel;
class QTreeView;

class NavigationPane : public QFrame
{
    Q_OBJECT

public:
    NavigationPane(QWidget* parent = nullptr);
    ~NavigationPane();

    void updateHeaders(const QString& htmlString);

private:
    void createGUI();

    QTreeView* m_tree_view;
    WikiHeaderListModel* m_model;
};
