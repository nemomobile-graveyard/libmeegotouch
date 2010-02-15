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

#ifndef DUITOOLBAR_STUB_H
#define DUITOOLBAR_STUB_H

#include <stubbase.h>
#include <duitoolbar.h>

class DuiToolbarStub : public StubBase
{
public:
    virtual void duiToolbarConstructor(DuiWidget *parent = 0);
    virtual void duiToolbarDestructor();
};

void DuiToolbarStub::duiToolbarConstructor(DuiWidget *parent)
{
    Q_UNUSED(parent);
}

void DuiToolbarStub::duiToolbarDestructor()
{
}

DuiToolbarStub gDefaultDuiToolbarStub;
DuiToolbarStub *gDuiToolbarStub = &gDefaultDuiToolbarStub;

DuiToolbar::DuiToolbar(DuiWidget *parent)
{
    gDuiToolbarStub->duiToolbarConstructor(parent);
}

DuiToolbar::~DuiToolbar()
{
    gDuiToolbarStub->duiToolbarDestructor();
}

#endif
