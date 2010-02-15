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

#include <duiapplication.h>

DuiApplication::DuiApplication(int &argc, char **argv)
    : QApplication(argc, argv)
{
}

bool DuiApplication::x11EventFilter(XEvent *event)
{
    Q_UNUSED(event);
    return false;
}


DuiApplication::~DuiApplication()
{
}

bool DuiApplication::showBoundingRect()
{
    return false;
}

bool DuiApplication::showFps()
{
    return false;
}

bool DuiApplication::softwareRendering()
{
    return false;
}

bool DuiApplication::portrait()
{
    return false;
}

void DuiApplication::updateSettings()
{
}
