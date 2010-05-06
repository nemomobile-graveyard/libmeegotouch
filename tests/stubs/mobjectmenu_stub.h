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

#ifndef MOBJECTMENU_STUB
#define MOBJECTMENU_STUB

#include "mobjectmenu.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MObjectMenuStub : public StubBase
{
public:
    virtual void MObjectMenuConstructor(MWidget *target);
    virtual void MObjectMenuDestructor();
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

// 2. IMPLEMENT STUB
void MObjectMenuStub::MObjectMenuConstructor(MWidget *target)
{
    Q_UNUSED(target);

}
void MObjectMenuStub::MObjectMenuDestructor()
{

}
bool MObjectMenuStub::eventFilter(QObject *obj, QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QObject * >(obj));
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("eventFilter", params);
    return stubReturnValue<bool>("eventFilter");
}

void MObjectMenuStub::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneContextMenuEvent * >(event));
    stubMethodEntered("contextMenuEvent", params);
}



// 3. CREATE A STUB INSTANCE
MObjectMenuStub gDefaultMObjectMenuStub;
MObjectMenuStub *gMObjectMenuStub = &gDefaultMObjectMenuStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MObjectMenu::MObjectMenu(MWidget *target)
{
    gMObjectMenuStub->MObjectMenuConstructor(target);
}

MObjectMenu::~MObjectMenu()
{
    gMObjectMenuStub->MObjectMenuDestructor();
}

bool MObjectMenu::eventFilter(QObject *obj, QEvent *event)
{
    return gMObjectMenuStub->eventFilter(obj, event);
}

void MObjectMenu::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    gMObjectMenuStub->contextMenuEvent(event);
}


#endif
