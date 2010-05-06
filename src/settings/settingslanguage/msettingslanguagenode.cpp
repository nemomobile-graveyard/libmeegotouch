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

#include "msettingslanguagenode.h"

MSettingsLanguageNode::MSettingsLanguageNode() :
    _children()
{
}

MSettingsLanguageNode::~MSettingsLanguageNode()
{
    // Delete the children to free memory
    foreach(MSettingsLanguageNode * child, _children) {
        delete child;
    }
}

void MSettingsLanguageNode::addChild(MSettingsLanguageNode *child)
{
    _children.append(child);
}

uint MSettingsLanguageNode::numChildren() const
{
    return _children.count();
}

QList<MSettingsLanguageNode *> MSettingsLanguageNode::children() const
{
    return _children;
}
