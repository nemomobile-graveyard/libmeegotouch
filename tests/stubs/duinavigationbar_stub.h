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

#ifndef DUINAVIGATIONBAR_STUB
#define DUINAVIGATIONBAR_STUB

#include "duinavigationbar.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiNavigationBarStub : public StubBase
{
public:
    virtual void DuiNavigationBarConstructor(QGraphicsWidget *parent);
    virtual void DuiNavigationBarDestructor();
    virtual void updateStyle();
    virtual void dockToolbar(DuiToolbar *toolbar);
    virtual DuiToolbar *undockToolbar();
    virtual bool isFullscreen() const;
    virtual void setFullscreen(bool isFullscreen);
    virtual void animateOrientationChange(qreal f, DuiOrientationChangeParameters *parameters);
    virtual void finalizeOrientationChange(DuiOrientationChangeParameters *parameters);
    DuiNavigationBarPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void DuiNavigationBarStub::DuiNavigationBarConstructor(QGraphicsWidget *parent)
{
    Q_UNUSED(parent);

}
void DuiNavigationBarStub::DuiNavigationBarDestructor()
{

}
void DuiNavigationBarStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

void DuiNavigationBarStub::dockToolbar(DuiToolbar *toolbar)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiToolbar * >(toolbar));
    stubMethodEntered("dockToolbar", params);
}

DuiToolbar *DuiNavigationBarStub::undockToolbar()
{
    stubMethodEntered("undockToolbar");
    return stubReturnValue<DuiToolbar *>("undockToolbar");
}

bool DuiNavigationBarStub::isFullscreen() const
{
    stubMethodEntered("isFullscreen");
    return stubReturnValue<bool>("isFullscreen");
}

void DuiNavigationBarStub::setFullscreen(bool isFullscreen)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(isFullscreen));
    stubMethodEntered("setFullscreen", params);
}

void DuiNavigationBarStub::animateOrientationChange(qreal f, DuiOrientationChangeParameters *parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(f));
    params.append(new Parameter<DuiOrientationChangeParameters * >(parameters));
    stubMethodEntered("animateOrientationChange", params);
}

void DuiNavigationBarStub::finalizeOrientationChange(DuiOrientationChangeParameters *parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiOrientationChangeParameters * >(parameters));
    stubMethodEntered("finalizeOrientationChange", params);
}

// 3. CREATE A STUB INSTANCE
DuiNavigationBarStub gDefaultDuiNavigationBarStub;
DuiNavigationBarStub *gDuiNavigationBarStub = &gDefaultDuiNavigationBarStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiNavigationBar::DuiNavigationBar(QGraphicsWidget *parent)
{
    gDuiNavigationBarStub->DuiNavigationBarConstructor(parent);
}

DuiNavigationBar::~DuiNavigationBar()
{
    gDuiNavigationBarStub->DuiNavigationBarDestructor();
}

void DuiNavigationBar::updateStyle()
{
    gDuiNavigationBarStub->updateStyle();
}

void DuiNavigationBar::dockToolbar(DuiToolbar *toolbar)
{
    gDuiNavigationBarStub->dockToolbar(toolbar);
}

DuiToolbar *DuiNavigationBar::undockToolbar()
{
    return gDuiNavigationBarStub->undockToolbar();
}

bool DuiNavigationBar::isFullscreen() const
{
    return gDuiNavigationBarStub->isFullscreen();
}

void DuiNavigationBar::setFullscreen(bool isFullscreen)
{
    gDuiNavigationBarStub->setFullscreen(isFullscreen);
}

void DuiNavigationBar::animateOrientationChange(qreal f, DuiOrientationChangeParameters *parameters)
{
    gDuiNavigationBarStub->animateOrientationChange(f, parameters);
}

void DuiNavigationBar::finalizeOrientationChange(DuiOrientationChangeParameters *parameters)
{
    gDuiNavigationBarStub->finalizeOrientationChange(parameters);
}


#endif
