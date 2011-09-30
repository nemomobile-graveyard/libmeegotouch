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
#include <MApplication>
#include <MApplicationPage>
#include <MApplicationWindow>
#include <MButton>
#include <MComponentCache>
#include <MExport>

#include "imageviewer.h"

M_EXPORT int main(int argc, char** argv)
{
    MApplication& app = *MComponentCache::mApplication(argc, argv);
    MApplicationWindow& w = *MComponentCache::mApplicationWindow();
    w.setObjectName( "Main Window" );
    w.show();

    MApplicationPage p;
    p.appear(&w);

    MButton b(p.centralWidget());
    b.setText("IMAGE 1");

    ImageViewer myImageViewer( b );

    return app.exec();
}
