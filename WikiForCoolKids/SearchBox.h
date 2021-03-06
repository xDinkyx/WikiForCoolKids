#pragma once

#include <QWidget>
#include <QThread>
#include <QStringList>

#include <memory>

class QLineEdit;
class WikiSearcher;

class SearchBox : public QWidget
{
    Q_OBJECT

public:
    SearchBox(QWidget* parent = nullptr);
    ~SearchBox();

    void clear();

signals:
    void startSearch(const QString& searchString, const QString& searchFolder);

private:
    void setupGui();
    void setInitialText();

    void search();

    bool eventFilter(QObject* object, QEvent* event) override;

    void displaySearchResult(const QStringList& results);

    QLineEdit* m_search_input;

    std::unique_ptr<WikiSearcher> m_searcher;
    QThread m_search_thread;
};
