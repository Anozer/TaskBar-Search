#ifndef SEARCHENGINE_H
#define SEARCHENGINE_H
#include <QString>
#include <QUrl>

struct SearchEngine {
    QString name;
    QString url;
};

Q_DECLARE_METATYPE(SearchEngine);

#endif // SEARCHENGINE_H
