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

#ifndef DUISCENE_STUB
#define DUISCENE_STUB

#include "duiscene.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiSceneStub : public StubBase
{
public:
    virtual void DuiSceneConstructor(QObject *parent);
    virtual void DuiSceneDestructor();
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual bool event(QEvent *event);
};

// 2. IMPLEMENT STUB
void DuiSceneStub::DuiSceneConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void DuiSceneStub::DuiSceneDestructor()
{

}

void DuiSceneStub::drawForeground(QPainter *painter, const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("drawForeground", params);
}

bool DuiSceneStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}



// 3. CREATE A STUB INSTANCE
DuiSceneStub gDefaultDuiSceneStub;
DuiSceneStub *gDuiSceneStub = &gDefaultDuiSceneStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
DuiScene::DuiScene(QObject *parent)
    : d_ptr(0)
{
    gDuiSceneStub->DuiSceneConstructor(parent);
}

DuiScene::~DuiScene()
{
    gDuiSceneStub->DuiSceneDestructor();
}

void DuiScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    gDuiSceneStub->drawForeground(painter, rect);
}

bool DuiScene::event(QEvent *event)
{
    return gDuiSceneStub->event(event);
}


#endif
