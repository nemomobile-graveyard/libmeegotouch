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

#ifndef MSCENEWINDOWPRIVATE_STUB
#define MSCENEWINDOWPRIVATE_STUB

#include "mscenewindow_p.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSceneWindowPrivateStub : public StubBase
{
public:
    virtual void MSceneWindowPrivateConstructor();
    MSceneWindow::WindowType windowType ;
    MSceneWindow::DeletionPolicy policy ;
    Qt::Alignment alignment ;
    QPointF offset ;
    bool managedManually ;
    bool shown ;
    bool dismissed ;
};

// 2. IMPLEMENT STUB
void MSceneWindowPrivateStub::MSceneWindowPrivateConstructor()
{

}



// 3. CREATE A STUB INSTANCE
MSceneWindowPrivateStub gDefaultMSceneWindowPrivateStub;
MSceneWindowPrivateStub *gMSceneWindowPrivateStub = &gDefaultMSceneWindowPrivateStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MSceneWindowPrivate::MSceneWindowPrivate()
{
    gMSceneWindowPrivateStub->MSceneWindowPrivateConstructor();
}


#endif
