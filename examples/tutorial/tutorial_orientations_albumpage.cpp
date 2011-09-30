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
#include <MLayout>
#include <MLinearLayoutPolicy>

...

void AlbumPage::createContent()
{
    MLayout *layout = new MLayout;
    centralWidget()->setLayout(layout);

    // Build a vertical layout that holds the cover art and the "By: Artist" label.
    QGraphicsLinearLayout *coverAndArtistLayout = new QGraphicsLinearLayout(Qt::Vertical);

    MImageWidget *albumCover = new MImageWidget(new QImage(album->coverArtFilename));
    coverAndArtistLayout->addItem(albumCover);

    QString byArtist = QString("By: %1").arg(album->artist);
    coverAndArtistLayout->addItem(new MLabel(byArtist));

    // Build a vertical layout that will hold the list of songs.
    QGraphicsLinearLayout *songsLayout = new QGraphicsLinearLayout(Qt::Vertical);
    songsLayout->addItem(new MLabel("Songs:"));
    MLabel *songLabel;
    for (int i = 0; i < album->songs.count(); i++) {
        songLabel = new MLabel(album->songs.at(i));
        songsLayout->addItem(songLabel);
    }

    // When in landscape orientation, have the cover and the songs list
    // side-by-side.
    MLinearLayoutPolicy *landscapePolicy = new MLinearLayoutPolicy(layout, Qt::Horizontal);
    landscapePolicy->addItem(coverAndArtistLayout);
    landscapePolicy->addItem(songsLayout);
    layout->setLandscapePolicy(landscapePolicy);

    // When in portrait orientation, have the cover and the songs list
    // on top of each other.
    MLinearLayoutPolicy *portraitPolicy = new MLinearLayoutPolicy(layout, Qt::Vertical);
    portraitPolicy->addItem(coverAndArtistLayout);
    portraitPolicy->addItem(songsLayout);
    layout->setPortraitPolicy(portraitPolicy);
}
