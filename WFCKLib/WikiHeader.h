#pragma once

#include "wfcklib_global.h"

#include <QString>
#include <vector>

class WFCKLIB_EXPORT WikiHeader
{
public:
    WikiHeader(QString path, QString name, WikiHeader* parent = nullptr);
    ~WikiHeader();

    QString getAnchor() const { return m_anchor; }
    QString getName() const { return m_name; }
    WikiHeader* getParent() const { return m_parent; }
    const std::vector<WikiHeader*>& getChildren() { return m_child_headers; }

    int childCount() const;
    int row() const;
    int level() const; // amount of parents

    void addChild(WikiHeader* header);
    WikiHeader* childAt(int row) const;

    void setParent(WikiHeader* parent) { m_parent = parent; }

    WikiHeader* findHeaderByAnchor(const QString& anchor);

private:
    QString m_anchor;
    QString m_name;
    WikiHeader* m_parent;
    std::vector<WikiHeader*> m_child_headers; // ownership
};