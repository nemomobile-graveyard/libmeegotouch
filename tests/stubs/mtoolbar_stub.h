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

#ifndef MTOOLBAR_STUB_H
#define MTOOLBAR_STUB_H

#include <stubbase.h>
#include <mtoolbar.h>

class MToolbarStub : public StubBase
{
public:
    virtual void mToolbarConstructor(MWidget *parent = 0);
    virtual void mToolbarDestructor();
};

void MToolbarStub::mToolbarConstructor(MWidget *parent)
{
    Q_UNUSED(parent);
}

void MToolbarStub::mToolbarDestructor()
{
}

MToolbarStub gDefaultMToolbarStub;
MToolbarStub *gMToolbarStub = &gDefaultMToolbarStub;

MToolbar::MToolbar(MWidget *parent)
{
    gMToolbarStub->mToolbarConstructor(parent);
}

MToolbar::~MToolbar()
{
    gMToolbarStub->mToolbarDestructor();
}

#endif
