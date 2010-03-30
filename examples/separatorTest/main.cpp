/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

/*
 * An example of a minimalistic DirectUI application
 */

#include <DuiApplication>
#include <DuiApplicationWindow>
#include <DuiLinearLayoutPolicy>

#include "SeparatorTestPage.h"

int main(int argc, char **argv)
{
    /* The base class of all DirectUI applications */
    DuiApplication app(argc, argv);

    /* The application window is a top-level window that contains
       the Home and Back/Close framework controls, Navigation bar,
       View menu and Toolbar components */
    DuiApplicationWindow w;

    /* Pages represent one "view" of an application, into which you
       can add your application's contents. An application can have
       any number of pages with transitions between them */
    SeparatorTestPage p;

    p.appear();
    w.show();

    return app.exec();
}
