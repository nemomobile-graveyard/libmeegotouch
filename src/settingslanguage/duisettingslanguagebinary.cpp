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

#include <QStringList>
#include "duisettingslanguagebinary.h"
#include "duisettingslanguageselection.h"
#include "duisettingslanguageinteger.h"
#include "duisettingslanguagetext.h"
#include "duisettingslanguageboolean.h"

DuiSettingsLanguageBinary::DuiSettingsLanguageBinary()
{
}

DuiSettingsLanguageBinary::~DuiSettingsLanguageBinary()
{
}

QStringList DuiSettingsLanguageBinary::keys() const
{
    QStringList keys;
    QList<DuiSettingsLanguageNode *> nodes;

    for (nodes.append(const_cast<DuiSettingsLanguageBinary *>(this)); !nodes.isEmpty();) {
        // Process the first node in the node list
        const DuiSettingsLanguageNode *node = nodes.takeFirst();

        // Append all children of the node to the node list
        nodes.append(node->children());

        const DuiSettingsLanguageSelection *selectionNode;
        const DuiSettingsLanguageInteger *intNode;
        const DuiSettingsLanguageText *textNode;
        const DuiSettingsLanguageBoolean *booleanNode;

        // Append the key of the node being processed (if any) to the key list
        if ((selectionNode = dynamic_cast<const DuiSettingsLanguageSelection *>(node))) {
            keys.append(selectionNode->key());
        } else if ((intNode = dynamic_cast<const DuiSettingsLanguageInteger *>(node))) {
            keys.append(intNode->key());
        } else if ((textNode = dynamic_cast<const DuiSettingsLanguageText *>(node))) {
            keys.append(textNode->key());
        } else if ((booleanNode = dynamic_cast<const DuiSettingsLanguageBoolean *>(node))) {
            keys.append(booleanNode->key());
        }
    }

    return keys;
}

