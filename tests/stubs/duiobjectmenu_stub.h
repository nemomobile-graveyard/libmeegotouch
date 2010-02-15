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

#ifndef DUIOBJECTMENU_STUB
#define DUIOBJECTMENU_STUB

#include "duiobjectmenu.h"
#include "duiscenewindowprivate_stub.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiObjectMenuStub : public StubBase
{
public:
    virtual void DuiObjectMenuConstructor(DuiWidget *target);
    virtual void DuiObjectMenuDestructor();
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
};

// 2. IMPLEMENT STUB
void DuiObjectMenuStub::DuiObjectMenuConstructor(DuiWidget *target)
{
    Q_UNUSED(target);

}
void DuiObjectMenuStub::DuiObjectMenuDestructor()
{

}
bool DuiObjectMenuStub::eventFilter(QObject *obj, QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QObject * >(obj));
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("eventFilter", params);
    return stubReturnValue<bool>("eventFilter");
}

void DuiObjectMenuStub::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QGraphicsSceneContextMenuEvent * >(event));
    stubMethodEntered("contextMenuEvent", params);
}



// 3. CREATE A STUB INSTANCE
DuiObjectMenuStub gDefaultDuiObjectMenuStub;
DuiObjectMenuStub *gDuiObjectMenuStub = &gDefaultDuiObjectMenuStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiObjectMenu::DuiObjectMenu(DuiWidget *target) : DuiSceneWindow(new DuiSceneWindowPrivate, new DuiObjectMenuModel, DuiSceneWindow::ObjectMenu)
{
    gDuiObjectMenuStub->DuiObjectMenuConstructor(target);
}

DuiObjectMenu::~DuiObjectMenu()
{
    gDuiObjectMenuStub->DuiObjectMenuDestructor();
}

bool DuiObjectMenu::eventFilter(QObject *obj, QEvent *event)
{
    return gDuiObjectMenuStub->eventFilter(obj, event);
}

void DuiObjectMenu::contextMenuEvent(QGraphicsSceneContextMenuEvent *event)
{
    gDuiObjectMenuStub->contextMenuEvent(event);
}


#endif
