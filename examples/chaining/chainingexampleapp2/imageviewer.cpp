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

#include "imageviewer.h"

#include <QDebug>

#include <MButton>
#include <MApplication>
#include <MApplicationWindow>
#include <MApplicationPage>
#include <MSceneWindow>
#include <MApplicationIfProxy>
#include <MComponentCache>

ImageViewer::ImageViewer()
{
}

ImageViewer::~ImageViewer()
{
}

bool ImageViewer::showImage(const QString &uri, const QStringList &uriList)
{
    Q_UNUSED( uri );

    MApplicationWindow* mWin = MComponentCache::mApplicationWindow();
    mWin->setObjectName( "MApplicationWindow#2" );
    mWin->setAttribute( Qt::WA_DeleteOnClose, true );

    MApplicationPage *p = new MApplicationPage();
    mWin->show();
    p->appear( mWin, MSceneWindow::DestroyWhenDone );

    MButton *b = new MButton( p->centralWidget() );
    b->setText( uriList[0] );

    return true;
}
