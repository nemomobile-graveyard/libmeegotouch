/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include "pt_mtheme.h"

#include <mbenchmark.h>
#include <MTheme>

#include <QtTest/QtTest>

void Pt_MTheme::uncachedConstructor()
{
    MTheme *theme = 0;
    MBENCHMARK_ONCE (
        theme = new MTheme("widgetsgallery");
    )
    delete theme;
}

void Pt_MTheme::cachedConstructor()
{
    MTheme *theme = 0;
    MBENCHMARK_ONCE (
        theme = new MTheme("widgetsgallery");
    )
    delete theme;
}

QTEST_MAIN(Pt_MTheme)
