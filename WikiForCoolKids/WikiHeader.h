#pragma once

#include <QString>
#include <vector>

class WikiHeader
{
public:
    WikiHeader(QString path, QString name, WikiHeader* parent = nullptr);
    ~WikiHeader();

    QString getPath() const { return m_path; }
    QString getName() const { return m_name; }
    WikiHeader* getParent() const { return m_parent; }
    const std::vector<WikiHeader*>& getChildren() { return m_child_headers; }

    void addChild(WikiHeader* header);
    WikiHeader* childAt(int row) const;
    int childCount() const;
    int row() const;

private:
    QString m_path;
    QString m_name;
    WikiHeader* m_parent;
    std::vector<WikiHeader*> m_child_headers;
};