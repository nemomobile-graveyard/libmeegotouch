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

#include "pt_mcomponentdata.h"

#include <MComponentData>
#include <MApplication>

void Pt_MComponentData::constructor()
{
    int argc = 1;
    char *argv[argc];
    char appName[] = "./widgetsgallery";
    argv[0] = appName;

    MComponentData *componentData = 0;
    QBENCHMARK_ONCE {
        componentData = new MComponentData(argc, argv, appName);
    }
    delete componentData;
}

QTEST_MAIN(Pt_MComponentData)
