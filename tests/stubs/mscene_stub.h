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

#ifndef MSCENE_STUB
#define MSCENE_STUB

#include "mscene.h"
#include <stubbase.h>


// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MSceneStub : public StubBase
{
public:
    virtual void MSceneConstructor(QObject *parent);
    virtual void MSceneDestructor();
    virtual void drawForeground(QPainter *painter, const QRectF &rect);
    virtual bool event(QEvent *event);
};

// 2. IMPLEMENT STUB
void MSceneStub::MSceneConstructor(QObject *parent)
{
    Q_UNUSED(parent);

}
void MSceneStub::MSceneDestructor()
{

}

void MSceneStub::drawForeground(QPainter *painter, const QRectF &rect)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QPainter * >(painter));
    params.append(new Parameter<const QRectF & >(rect));
    stubMethodEntered("drawForeground", params);
}

bool MSceneStub::event(QEvent *event)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<QEvent * >(event));
    stubMethodEntered("event", params);
    return stubReturnValue<bool>("event");
}



// 3. CREATE A STUB INSTANCE
MSceneStub gDefaultMSceneStub;
MSceneStub *gMSceneStub = &gDefaultMSceneStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
MScene::MScene(QObject *parent)
    : d_ptr(0)
{
    gMSceneStub->MSceneConstructor(parent);
}

MScene::~MScene()
{
    gMSceneStub->MSceneDestructor();
}

void MScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    gMSceneStub->drawForeground(painter, rect);
}

bool MScene::event(QEvent *event)
{
    return gMSceneStub->event(event);
}


#endif
