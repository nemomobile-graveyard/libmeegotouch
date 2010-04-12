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

#ifndef MAPPLICATIONMENU_STUB_H
#define MAPPLICATIONMENU_STUB_H

#include <stubbase.h>
#include <mapplicationmenu.h>

class MApplicationMenuStub : public StubBase
{
public:
    virtual void mApplicationMenuConstructor(MWidgetController *parent = 0);
    virtual void mApplicationMenuDestructor();
};

void MApplicationMenuStub::mApplicationMenuConstructor(MWidgetController *parent)
{
    Q_UNUSED(parent);
}

void MApplicationMenuStub::mApplicationMenuDestructor()
{
}

MApplicationMenuStub gDefaultMApplicationMenuStub;
MApplicationMenuStub *gMApplicationMenuStub = &gDefaultMApplicationMenuStub;

MApplicationMenu::MApplicationMenu(MWidgetController *parent)
{
    gMApplicationMenuStub->mApplicationMenuConstructor(parent);
}

MApplicationMenu::~MApplicationMenu()
{
    gMApplicationMenuStub->mApplicationMenuDestructor();
}

#endif
