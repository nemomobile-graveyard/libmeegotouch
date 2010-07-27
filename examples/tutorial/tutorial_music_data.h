// data.h
#ifndef DATA_H
#define DATA_H

#include <QString>
#include <QStringList>

class Album {
public:
    QString title;
    QString artist;
    QString coverArtFilename;
    QStringList songs;
};

class Artist {
public:
    virtual ~Artist() { qDeleteAll(albums); }

    QString name;
    QList<Album *> albums;
};

#endif
