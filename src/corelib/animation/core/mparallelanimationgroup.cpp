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

#include "mparallelanimationgroup.h"
#include "mparallelanimationgroup_p.h"

#include "manimationstyle.h"
#include "mtheme.h"

// protected constructor
MParallelAnimationGroup::MParallelAnimationGroup(MParallelAnimationGroupPrivate *dd, QObject *parent) :
    QParallelAnimationGroup(parent),
    d_ptr(dd)
{
    Q_D(MParallelAnimationGroup);
    d->q_ptr = this;
    d->styleContainer = 0;
}

// public constructor
MParallelAnimationGroup::MParallelAnimationGroup(QObject *parent) :
    QParallelAnimationGroup(parent),
    d_ptr(new MParallelAnimationGroupPrivate)
{
    Q_D(MParallelAnimationGroup);
    d->q_ptr = this;
    d->styleContainer = 0;
}

// destructor
MParallelAnimationGroup::~MParallelAnimationGroup()
{
    Q_D(MParallelAnimationGroup);

    delete d->styleContainer;
    delete d_ptr;
}

MAnimationStyleContainer &MParallelAnimationGroup::style()
{
    Q_D(MParallelAnimationGroup);

    if (!d->styleContainer) {
        d->styleContainer = createStyleContainer();
        d->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const MAnimationStyleContainer &MParallelAnimationGroup::style() const
{
    Q_D(const MParallelAnimationGroup);

    if (!d->styleContainer) {
        MParallelAnimationGroupPrivate *d_p = const_cast<MParallelAnimationGroupPrivate *>(d);
        d_p->styleContainer = createStyleContainer();
        d_p->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const char *MParallelAnimationGroup::styleType() const
{
    return "MAnimationStyle";
}

MAnimationStyleContainer *MParallelAnimationGroup::createStyleContainer() const
{
    return new MAnimationStyleContainer();
}

