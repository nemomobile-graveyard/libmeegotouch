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

#include "manimation.h"
#include "manimation_p.h"

#include "manimationstyle.h"
#include "mtheme.h"

// protected constructor
MAnimation::MAnimation(MAnimationPrivate *dd, QObject *parent) :
    QAbstractAnimation(parent),
    d_ptr(dd)
{
    Q_D(MAnimation);
    d->q_ptr = this;
    d->styleContainer = 0;
}

// public constructor
MAnimation::MAnimation(QObject *parent) :
    QAbstractAnimation(parent),
    d_ptr(new MAnimationPrivate)
{
    Q_D(MAnimation);
    d->q_ptr = this;
    d->styleContainer = 0;
}

// destructor
MAnimation::~MAnimation()
{
    Q_D(MAnimation);

    delete d->styleContainer;
    delete d_ptr;
}

MAnimationStyleContainer &MAnimation::style()
{
    Q_D(MAnimation);

    if (!d->styleContainer) {
        d->styleContainer = createStyleContainer();
        d->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const MAnimationStyleContainer &MAnimation::style() const
{
    Q_D(const MAnimation);

    if (!d->styleContainer) {
        MAnimationPrivate *d_p = const_cast<MAnimationPrivate *>(d);
        d_p->styleContainer = createStyleContainer();
        d_p->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const char *MAnimation::styleType() const
{
    return "MAnimationStyle";
}

MAnimationStyleContainer *MAnimation::createStyleContainer() const
{
    return new MAnimationStyleContainer();
}

