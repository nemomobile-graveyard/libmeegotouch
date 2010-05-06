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

#ifndef MNAVIGATIONBAR_STUB
#define MNAVIGATIONBAR_STUB

#include "mnavigationbar.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MNavigationBarStub : public StubBase
{
public:
    virtual void MNavigationBarConstructor(QGraphicsWidget *parent);
    virtual void MNavigationBarDestructor();
    virtual void updateStyle();
    virtual void dockToolbar(MToolbar *toolbar);
    virtual MToolbar *undockToolbar();
    virtual bool isFullscreen() const;
    virtual void setFullscreen(bool isFullscreen);
    virtual void animateOrientationChange(qreal f, MOrientationChangeParameters *parameters);
    virtual void finalizeOrientationChange(MOrientationChangeParameters *parameters);
    MNavigationBarPrivate *d_ptr ;
};

// 2. IMPLEMENT STUB
void MNavigationBarStub::MNavigationBarConstructor(QGraphicsWidget *parent)
{
    Q_UNUSED(parent);

}
void MNavigationBarStub::MNavigationBarDestructor()
{

}
void MNavigationBarStub::updateStyle()
{
    stubMethodEntered("updateStyle");
}

void MNavigationBarStub::dockToolbar(MToolbar *toolbar)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MToolbar * >(toolbar));
    stubMethodEntered("dockToolbar", params);
}

MToolbar *MNavigationBarStub::undockToolbar()
{
    stubMethodEntered("undockToolbar");
    return stubReturnValue<MToolbar *>("undockToolbar");
}

bool MNavigationBarStub::isFullscreen() const
{
    stubMethodEntered("isFullscreen");
    return stubReturnValue<bool>("isFullscreen");
}

void MNavigationBarStub::setFullscreen(bool isFullscreen)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<bool >(isFullscreen));
    stubMethodEntered("setFullscreen", params);
}

void MNavigationBarStub::animateOrientationChange(qreal f, MOrientationChangeParameters *parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<qreal >(f));
    params.append(new Parameter<MOrientationChangeParameters * >(parameters));
    stubMethodEntered("animateOrientationChange", params);
}

void MNavigationBarStub::finalizeOrientationChange(MOrientationChangeParameters *parameters)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MOrientationChangeParameters * >(parameters));
    stubMethodEntered("finalizeOrientationChange", params);
}

// 3. CREATE A STUB INSTANCE
MNavigationBarStub gDefaultMNavigationBarStub;
MNavigationBarStub *gMNavigationBarStub = &gDefaultMNavigationBarStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MNavigationBar::MNavigationBar(QGraphicsWidget *parent)
{
    gMNavigationBarStub->MNavigationBarConstructor(parent);
}

MNavigationBar::~MNavigationBar()
{
    gMNavigationBarStub->MNavigationBarDestructor();
}

void MNavigationBar::updateStyle()
{
    gMNavigationBarStub->updateStyle();
}

void MNavigationBar::dockToolbar(MToolbar *toolbar)
{
    gMNavigationBarStub->dockToolbar(toolbar);
}

MToolbar *MNavigationBar::undockToolbar()
{
    return gMNavigationBarStub->undockToolbar();
}

bool MNavigationBar::isFullscreen() const
{
    return gMNavigationBarStub->isFullscreen();
}

void MNavigationBar::setFullscreen(bool isFullscreen)
{
    gMNavigationBarStub->setFullscreen(isFullscreen);
}

void MNavigationBar::animateOrientationChange(qreal f, MOrientationChangeParameters *parameters)
{
    gMNavigationBarStub->animateOrientationChange(f, parameters);
}

void MNavigationBar::finalizeOrientationChange(MOrientationChangeParameters *parameters)
{
    gMNavigationBarStub->finalizeOrientationChange(parameters);
}


#endif
