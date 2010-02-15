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

#include "duiparallelanimationgroup.h"
#include "duiparallelanimationgroup_p.h"

#include "duianimationstyle.h"
#include "duitheme.h"

// protected constructor
DuiParallelAnimationGroup::DuiParallelAnimationGroup(DuiParallelAnimationGroupPrivate *dd, QObject *parent) :
    QParallelAnimationGroup(parent),
    d_ptr(dd)
{
    Q_D(DuiParallelAnimationGroup);
    d->q_ptr = this;
    d->styleContainer = 0;
}

// public constructor
DuiParallelAnimationGroup::DuiParallelAnimationGroup(QObject *parent) :
    QParallelAnimationGroup(parent),
    d_ptr(new DuiParallelAnimationGroupPrivate)
{
    Q_D(DuiParallelAnimationGroup);
    d->q_ptr = this;
    d->styleContainer = 0;
}

// destructor
DuiParallelAnimationGroup::~DuiParallelAnimationGroup()
{
    Q_D(DuiParallelAnimationGroup);

    delete d->styleContainer;
    delete d_ptr;
}

DuiAnimationStyleContainer &DuiParallelAnimationGroup::style()
{
    Q_D(DuiParallelAnimationGroup);

    if (!d->styleContainer) {
        d->styleContainer = createStyleContainer();
        d->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const DuiAnimationStyleContainer &DuiParallelAnimationGroup::style() const
{
    Q_D(const DuiParallelAnimationGroup);

    if (!d->styleContainer) {
        DuiParallelAnimationGroupPrivate *d_p = const_cast<DuiParallelAnimationGroupPrivate *>(d);
        d_p->styleContainer = createStyleContainer();
        d_p->styleContainer->initialize(objectName(), "", NULL);
    }

    return *d->styleContainer;
}

const char *DuiParallelAnimationGroup::styleType() const
{
    return "DuiAnimationStyle";
}

DuiAnimationStyleContainer *DuiParallelAnimationGroup::createStyleContainer() const
{
    return new DuiAnimationStyleContainer();
}

