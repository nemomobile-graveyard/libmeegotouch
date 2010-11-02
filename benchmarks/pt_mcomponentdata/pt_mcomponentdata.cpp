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

#include <mbenchmark.h>
#include <MComponentData>
#include <MApplication>
#include <MApplicationService>
#include <MComponentCache>

namespace {
    int argc = 1;
    char appName[] = "./pt_mcomponentdata";
    char *argv[] = { appName };
}

void Pt_MComponentData::uncachedConstructor()
{
    MComponentData *componentData = 0;
    MBENCHMARK_ONCE(
        componentData = MComponentData::createInstance(argc, argv, argv[0]);
    )
    delete componentData;
}

void Pt_MComponentData::cachedConstructor()
{
    MComponentData *componentData = 0;
    MBENCHMARK_ONCE(
        componentData = MComponentData::createInstance(argc, argv, argv[0]);
    )
    delete componentData;
}

QTEST_MAIN(Pt_MComponentData)
