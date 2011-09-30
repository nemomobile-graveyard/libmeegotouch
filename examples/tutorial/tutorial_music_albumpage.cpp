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
// Filename: albumpage.cpp
#include "albumpage.h"

#include <MLabel>
#include <MBasicListItem>
#include <MImageWidget>
#include <QGraphicsLinearLayout>

AlbumPage::AlbumPage(const Album *album, QGraphicsItem *parent)
    : MApplicationPage(parent), album(album)
{
    setTitle(album->title);
}

void AlbumPage::createContent()
{
    QGraphicsLinearLayout *layout = new QGraphicsLinearLayout(Qt::Vertical);
    centralWidget()->setLayout(layout);

    MImageWidget *albumCover = new MImageWidget(new QImage(album->coverArtFilename));
    layout->addItem(albumCover);

    QString byArtist = QString("By: %1").arg(album->artist);
    layout->addItem(new MLabel(byArtist));

    layout->addItem(new MLabel("Songs:"));

    MLabel *songLabel;
    for (int i = 0; i < album->songs.count(); i++) {
        songLabel = new MLabel(album->songs.at(i));
        layout->addItem(songLabel);
    }
}
