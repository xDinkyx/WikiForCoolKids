#include "WikiHeader.h"

WikiHeader::WikiHeader(QString path, QString name, WikiHeader* parent /*= nullptr*/)
    : m_path(path)
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


