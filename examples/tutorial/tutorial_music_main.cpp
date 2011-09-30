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
#include <MApplicationWindow>

// The definition of our data classes
#include "data.h"

#include "mainpage.h"

void fillOutData(QList<Artist *> &artistList);

int main(int argc, char **argv)
{
    MApplication app(argc, argv);
    MApplicationWindow window;
    MainPage *mainPage;

    // That's the data that will be displayed by our application.
    // For the sake of keeping the example as simple as possible we use
    // a very simplistic data structure.
    QList<Artist *> artistsList;
    fillOutData(artistsList);

    mainPage = new MainPage(artistsList);

    mainPage->appear(&window);
    window.show();

    return app.exec();
}

void fillOutData(QList<Artist *> &artistList)
{
    ...
}
