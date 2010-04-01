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

#include "duianimation.h"
#include "duianimation_p.h"

#include "duianimationstyle.h"
#include "duitheme.h"

// protected constructor
DuiAnimation::DuiAnimation(DuiAnimationPrivate *dd, QObject *parent) :
    QAbstractAnimation(parent),
    d_ptr(dd)
{
    Q_D(DuiAnimation);
    d->q_ptr = this;
    d->styleContainer = 0;
}

// public constructor
DuiAnimation::DuiAnimation(QObject *parent) :
    QAbstractAnimation(parent),
    d_ptr(new DuiAnimationPrivate)
{
    Q_D(DuiAnimation);
    d->q_ptr = this;
    d->styleContainer = 0;
}

// destructor
DuiAnimation::~DuiAnimation()
{
    Q_D(DuiAnimation);

    delete d->styleContainer;
    delete d_ptr;
}

DuiAnimationStyleContainer &DuiAnimation::style()
{
    Q_D(DuiAnimation);

    if (!d->styleContainer) {
        d->styleContainer = createStyleContainer();
        d->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const DuiAnimationStyleContainer &DuiAnimation::style() const
{
    Q_D(const DuiAnimation);

    if (!d->styleContainer) {
        DuiAnimationPrivate *d_p = const_cast<DuiAnimationPrivate *>(d);
        d_p->styleContainer = createStyleContainer();
        d_p->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const char *DuiAnimation::styleType() const
{
    return "DuiAnimationStyle";
}

DuiAnimationStyleContainer *DuiAnimation::createStyleContainer() const
{
    return new DuiAnimationStyleContainer();
}

