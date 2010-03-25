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

#ifndef DUISCENEWINDOWPRIVATE_STUB
#define DUISCENEWINDOWPRIVATE_STUB

#include "duiscenewindow_p.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSceneWindowPrivateStub : public StubBase
{
public:
    virtual void DuiSceneWindowPrivateConstructor();
    DuiSceneWindow::WindowType windowType ;
    DuiSceneWindow::DeletionPolicy policy ;
    Qt::Alignment alignment ;
    QPointF offset ;
    bool managedManually ;
    bool shown ;
    bool dismissed ;
};

// 2. IMPLEMENT STUB
void DuiSceneWindowPrivateStub::DuiSceneWindowPrivateConstructor()
{

}



// 3. CREATE A STUB INSTANCE
DuiSceneWindowPrivateStub gDefaultDuiSceneWindowPrivateStub;
DuiSceneWindowPrivateStub *gDuiSceneWindowPrivateStub = &gDefaultDuiSceneWindowPrivateStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiSceneWindowPrivate::DuiSceneWindowPrivate()
{
    gDuiSceneWindowPrivateStub->DuiSceneWindowPrivateConstructor();
}


#endif
