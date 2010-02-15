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

#include "duiappletobjectmenumessage.h"
#include <QAction>

DuiAppletObjectMenuMessage::DuiAppletObjectMenuMessage(const QList<QAction *> actions) :
    DuiAppletMessage(OBJECT_MENU_MESSAGE)
{
    foreach(QAction * action, actions) {
        actionNamesList.append(action->text());
    }
}

DuiAppletObjectMenuMessage::~DuiAppletObjectMenuMessage()
{
}

void DuiAppletObjectMenuMessage::serialize(QDataStream &stream) const
{
    stream << actionNamesList;
}

void DuiAppletObjectMenuMessage::unserialize(QDataStream &stream)
{
    actionNamesList.clear();
    stream >> actionNamesList;
}

QList<QString> DuiAppletObjectMenuMessage::actionList() const
{
    return actionNamesList ;
}
