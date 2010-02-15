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

#ifndef DUIPARTICLECLOUD_H
#define DUIPARTICLECLOUD_H

#include <QtGlobal>
#if QT_VERSION >= 0x040600

#include "particle/duiparticleengine.h"


class DuiParticleCloud : public DuiParticleEngine
{
public:
    DuiParticleCloud(QObject *parent);
    virtual ~DuiParticleCloud();

    virtual int duration() const;
    void setPos(const QPointF &pos);

protected:
    virtual void initParticle(DuiParticle &p, int curmsecs);
    virtual void stepParticle(DuiParticle &p, int dt, int msecs);
private:
    QPointF position;
};

#endif

#endif

