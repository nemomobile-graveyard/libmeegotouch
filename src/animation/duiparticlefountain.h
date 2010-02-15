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

#ifndef DUIPARTICLEFOUNTAIN_H
#define DUIPARTICLEFOUNTAIN_H

#include "particle/duiparticleengine.h"

#include <QtGlobal>
#if QT_VERSION >= 0x040600

class DuiParticleFountain : public DuiParticleEngine
{
public:
    DuiParticleFountain(QObject *parent);
    virtual ~DuiParticleFountain();

    virtual int duration() const;

protected:
    virtual void initParticle(DuiParticle &p, int curmsecs);
};

#endif

#endif

