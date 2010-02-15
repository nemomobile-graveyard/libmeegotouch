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

#ifndef DUIPOSITIONINDICATORVIEW_STUB
#define DUIPOSITIONINDICATORVIEW_STUB

#include "duipositionindicatorview.h"
#include <stubbase.h>


// 1. DECLARE STUB
class DuiPositionIndicatorViewStub : public StubBase
{
public:
    virtual void DuiPositionIndicatorViewConstructor(DuiPositionIndicator *controller);
    virtual void DuiPositionIndicatorViewDestructor();
    virtual QRectF boundingRect() const;
    virtual QPainterPath shape() const;
//    virtual void styleUpdated();
    //DuiPositionIndicatorViewPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiPositionIndicatorViewStub::DuiPositionIndicatorViewConstructor(DuiPositionIndicator *controller)
{
    Q_UNUSED(controller);

}
void DuiPositionIndicatorViewStub::DuiPositionIndicatorViewDestructor()
{

}


QRectF DuiPositionIndicatorViewStub::boundingRect() const
{
    stubMethodEntered("boundingRect");
    return stubReturnValue<QRectF>("boundingRect");
}

QPainterPath DuiPositionIndicatorViewStub::shape() const
{
    stubMethodEntered("shape");
    return stubReturnValue<QPainterPath>("shape");
}
/*void DuiPositionIndicatorViewStub::styleUpdated()
{
   stubMethodEntered("styleUpdated");
}
*/
// 3. CREATE A STUB INSTANCE
DuiPositionIndicatorViewStub gDefaultDuiPositionIndicatorViewStub;
DuiPositionIndicatorViewStub *gDuiPositionIndicatorViewStub = &gDefaultDuiPositionIndicatorViewStub;

// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiPositionIndicatorView::DuiPositionIndicatorView(DuiPositionIndicator *controller)
{
    gDuiPositionIndicatorViewStub->DuiPositionIndicatorViewConstructor(controller);
}

DuiPositionIndicatorView::~DuiPositionIndicatorView()
{
    gDuiPositionIndicatorViewStub->DuiPositionIndicatorViewDestructor();
}


QRectF DuiPositionIndicatorView::boundingRect() const
{
    return gDuiPositionIndicatorViewStub->boundingRect();
}

QPainterPath DuiPositionIndicatorView::shape() const
{
    return gDuiPositionIndicatorViewStub->shape();
}
void DuiPositionIndicatorView::styleUpdated()
{
//    gDuiPositionIndicatorViewStub->styleUpdated();
}

#endif

