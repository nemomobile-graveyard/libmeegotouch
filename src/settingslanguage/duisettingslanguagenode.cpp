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

#include "duisettingslanguagenode.h"

DuiSettingsLanguageNode::DuiSettingsLanguageNode() :
    _children()
{
}

DuiSettingsLanguageNode::~DuiSettingsLanguageNode()
{
    // Delete the children to free memory
    foreach(DuiSettingsLanguageNode * child, _children) {
        delete child;
    }
}

void DuiSettingsLanguageNode::addChild(DuiSettingsLanguageNode *child)
{
    _children.append(child);
}

uint DuiSettingsLanguageNode::numChildren() const
{
    return _children.count();
}

QList<DuiSettingsLanguageNode *> DuiSettingsLanguageNode::children() const
{
    return _children;
}
