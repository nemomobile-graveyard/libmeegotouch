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
 

#include "mgraphicseffect.h"
#include "mgraphicseffect_p.h"

MGraphicsEffectPrivate::MGraphicsEffectPrivate()
    : q_ptr(0),
      styleContainer(0)
{
}

MGraphicsEffectPrivate::~MGraphicsEffectPrivate()
{
    delete styleContainer;
}

MGraphicsEffect::MGraphicsEffect(QObject *parent)
    : QGraphicsEffect(parent),
    d_ptr(new MGraphicsEffectPrivate)
{
    Q_D(MGraphicsEffect);
    d->q_ptr = this;
}

MGraphicsEffect::~MGraphicsEffect()
{
    delete d_ptr;
}

MGraphicsEffectStyleContainer &MGraphicsEffect::style()
{
    Q_D(MGraphicsEffect);

    if (!d->styleContainer) {
        d->styleContainer = createStyleContainer();
        d->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const MGraphicsEffectStyleContainer &MGraphicsEffect::style() const
{
    Q_D(const MGraphicsEffect);

    if (!d->styleContainer) {
        MGraphicsEffectPrivate *d_p = const_cast<MGraphicsEffectPrivate *>(d);
        d_p->styleContainer = createStyleContainer();
        d_p->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const char *MGraphicsEffect::styleType() const
{
    return "MGraphicsEffectStyle";
}

MGraphicsEffectStyleContainer *MGraphicsEffect::createStyleContainer() const
{
    return new MGraphicsEffectStyleContainer();
}
