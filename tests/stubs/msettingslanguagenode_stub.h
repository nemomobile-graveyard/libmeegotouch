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

#ifndef MSETTINGSLANGUAGENODE_STUB
#define MSETTINGSLANGUAGENODE_STUB

#include "msettingslanguagenode.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSettingsLanguageNodeStub : public StubBase
{
public:
    virtual void MSettingsLanguageNodeConstructor();
    virtual void MSettingsLanguageNodeDestructor();
    virtual void addChild(MSettingsLanguageNode *child);
    virtual uint numChildren() const;
    virtual QList<MSettingsLanguageNode *> children() const;
};

// 2. IMPLEMENT STUB
void MSettingsLanguageNodeStub::MSettingsLanguageNodeConstructor()
{
}

void MSettingsLanguageNodeStub::MSettingsLanguageNodeDestructor()
{
}

void MSettingsLanguageNodeStub::addChild(MSettingsLanguageNode *child)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MSettingsLanguageNode * >(child));
    stubMethodEntered("addChild", params);
}

uint MSettingsLanguageNodeStub::numChildren() const
{
    stubMethodEntered("numChildren");
    return stubReturnValue<uint>("numChildren");
}

QList<MSettingsLanguageNode *> MSettingsLanguageNodeStub::children() const
{
    stubMethodEntered("children");
    return stubReturnValue<QList<MSettingsLanguageNode *> >("children");
}


// 3. CREATE A STUB INSTANCE
MSettingsLanguageNodeStub gDefaultMSettingsLanguageNodeStub;
MSettingsLanguageNodeStub *gMSettingsLanguageNodeStub = &gDefaultMSettingsLanguageNodeStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSettingsLanguageNode::MSettingsLanguageNode()
{
    gMSettingsLanguageNodeStub->MSettingsLanguageNodeConstructor();
}

MSettingsLanguageNode::~MSettingsLanguageNode()
{
    gMSettingsLanguageNodeStub->MSettingsLanguageNodeDestructor();
}

void MSettingsLanguageNode::addChild(MSettingsLanguageNode *child)
{
    gMSettingsLanguageNodeStub->addChild(child);
}

uint MSettingsLanguageNode::numChildren() const
{
    return gMSettingsLanguageNodeStub->numChildren();
}

QList<MSettingsLanguageNode *> MSettingsLanguageNode::children() const
{
    return gMSettingsLanguageNodeStub->children();
}

#endif
