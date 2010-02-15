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

#ifndef DUIAPPLICATIONMENU_STUB_H
#define DUIAPPLICATIONMENU_STUB_H

#include <stubbase.h>
#include <duiapplicationmenu.h>

class DuiApplicationMenuStub : public StubBase
{
public:
    virtual void duiApplicationMenuConstructor(DuiWidgetController *parent = 0);
    virtual void duiApplicationMenuDestructor();
};

void DuiApplicationMenuStub::duiApplicationMenuConstructor(DuiWidgetController *parent)
{
    Q_UNUSED(parent);
}

void DuiApplicationMenuStub::duiApplicationMenuDestructor()
{
}

DuiApplicationMenuStub gDefaultDuiApplicationMenuStub;
DuiApplicationMenuStub *gDuiApplicationMenuStub = &gDefaultDuiApplicationMenuStub;

DuiApplicationMenu::DuiApplicationMenu(DuiWidgetController *parent)
{
    gDuiApplicationMenuStub->duiApplicationMenuConstructor(parent);
}

DuiApplicationMenu::~DuiApplicationMenu()
{
    gDuiApplicationMenuStub->duiApplicationMenuDestructor();
}

#endif
