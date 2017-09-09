#include "WikiHeader.h"

#include <QtGlobal>

WikiHeader::WikiHeader(QString path, QString name, WikiHeader* parent /*= nullptr*/)
    : m_anchor(path)
    , m_name(name)
    , m_parent(parent)
{
    if(m_parent != nullptr)
        m_parent->addChild(this);
}

WikiHeader::~WikiHeader()
{
    for (WikiHeader* child : m_child_headers)
    {
        delete child;
    }
}

void WikiHeader::addChild(WikiHeader* header)
{
    m_child_headers.push_back(header);
    header->setParent(this);
}

WikiHeader* WikiHeader::childAt(int row) const
{
    return m_child_headers[row];
}

int WikiHeader::childCount() const
{
    return m_child_headers.size();
}

int WikiHeader::row() const
{
    if (m_parent == nullptr)
        return 0;

    return std::find(m_parent->getChildren().begin(), m_parent->getChildren().end(), this) - m_parent->getChildren().begin();
}

int WikiHeader::level() const
{
    WikiHeader* parent(m_parent);
    int level(0);

    // Go up tree untill no parent is found (top header)
    while (parent != nullptr)
    {
        ++level;
        parent = parent->getParent();
    }

    return level;
}


