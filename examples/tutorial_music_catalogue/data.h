/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
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
