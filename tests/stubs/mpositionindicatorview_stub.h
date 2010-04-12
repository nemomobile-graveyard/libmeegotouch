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

#ifndef MPOSITIONINDICATORVIEW_STUB
#define MPOSITIONINDICATORVIEW_STUB

#include "mpositionindicatorview.h"
#include <stubbase.h>


// 1. DECLARE STUB
class MPositionIndicatorViewStub : public StubBase
{
public:
    virtual void MPositionIndicatorViewConstructor(MPositionIndicator *controller);
    virtual void MPositionIndicatorViewDestructor();
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
//    virtual void styleUpdated();
    //MPositionIndicatorViewPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MPositionIndicatorViewStub::MPositionIndicatorViewConstructor(MPositionIndicator *controller)
{
    Q_UNUSED(controller);

}
void MPositionIndicatorViewStub::MPositionIndicatorViewDestructor()
{

}


QRectF MPositionIndicatorViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

QPainterPath MPositionIndicatorViewStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}
/*void MPositionIndicatorViewStub::styleUpdated()
{
   stubMethodEntered("styleUpdated");
}
*/
// 3. CREATE A STUB INSTANCE
MPositionIndicatorViewStub gDefaultMPositionIndicatorViewStub;
MPositionIndicatorViewStub *gMPositionIndicatorViewStub = &gDefaultMPositionIndicatorViewStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
MPositionIndicatorView::MPositionIndicatorView(MPositionIndicator *controller)
{
    gMPositionIndicatorViewStub->MPositionIndicatorViewConstructor(controller);
}

MPositionIndicatorView::~MPositionIndicatorView()
{
    gMPositionIndicatorViewStub->MPositionIndicatorViewDestructor();
}


QRectF MPositionIndicatorView::boundingRect() const
{
    return gMPositionIndicatorViewStub->boundingRect();
}

QPainterPath MPositionIndicatorView::shape() const
{
    return gMPositionIndicatorViewStub->shape();
}
void MPositionIndicatorView::styleUpdated()
{
//    gMPositionIndicatorViewStub->styleUpdated();
}

#endif

