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

#ifndef DUISETTINGSLANGUAGENODE_STUB
#define DUISETTINGSLANGUAGENODE_STUB

#include "duisettingslanguagenode.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSettingsLanguageNodeStub : public StubBase
{
public:
    virtual void DuiSettingsLanguageNodeConstructor();
    virtual void DuiSettingsLanguageNodeDestructor();
    virtual void addChild(DuiSettingsLanguageNode *child);
    virtual uint numChildren() const;
    virtual QList<DuiSettingsLanguageNode *> children() const;
};

// 2. IMPLEMENT STUB
void DuiSettingsLanguageNodeStub::DuiSettingsLanguageNodeConstructor()
{
}

void DuiSettingsLanguageNodeStub::DuiSettingsLanguageNodeDestructor()
{
}

void DuiSettingsLanguageNodeStub::addChild(DuiSettingsLanguageNode *child)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiSettingsLanguageNode * >(child));
    stubMethodEntered("addChild", params);
}

uint DuiSettingsLanguageNodeStub::numChildren() const
{
    stubMethodEntered("numChildren");
    return stubReturnValue<uint>("numChildren");
}

QList<DuiSettingsLanguageNode *> DuiSettingsLanguageNodeStub::children() const
{
    stubMethodEntered("children");
    return stubReturnValue<QList<DuiSettingsLanguageNode *> >("children");
}


// 3. CREATE A STUB INSTANCE
DuiSettingsLanguageNodeStub gDefaultDuiSettingsLanguageNodeStub;
DuiSettingsLanguageNodeStub *gDuiSettingsLanguageNodeStub = &gDefaultDuiSettingsLanguageNodeStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSettingsLanguageNode::DuiSettingsLanguageNode()
{
    gDuiSettingsLanguageNodeStub->DuiSettingsLanguageNodeConstructor();
}

DuiSettingsLanguageNode::~DuiSettingsLanguageNode()
{
    gDuiSettingsLanguageNodeStub->DuiSettingsLanguageNodeDestructor();
}

void DuiSettingsLanguageNode::addChild(DuiSettingsLanguageNode *child)
{
    gDuiSettingsLanguageNodeStub->addChild(child);
}

uint DuiSettingsLanguageNode::numChildren() const
{
    return gDuiSettingsLanguageNodeStub->numChildren();
}

QList<DuiSettingsLanguageNode *> DuiSettingsLanguageNode::children() const
{
    return gDuiSettingsLanguageNodeStub->children();
}

#endif
