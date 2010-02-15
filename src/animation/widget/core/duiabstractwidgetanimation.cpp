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

#include "duiabstractwidgetanimation.h"
#include "duiabstractwidgetanimation_p.h"

#include "duiwidgetview.h"
#include "private/duiwidgetview_p.h"

DuiAbstractWidgetAnimation::DuiAbstractWidgetAnimation(DuiAbstractWidgetAnimationPrivate *dd, QObject *parent) :
    DuiAnimation(dd, parent)
{
}

DuiAbstractWidgetAnimation::DuiAbstractWidgetAnimation(QObject *parent) :
    DuiAnimation(new DuiAbstractWidgetAnimationPrivate, parent)
{
}

DuiAbstractWidgetAnimation::~DuiAbstractWidgetAnimation()
{
}

void DuiAbstractWidgetAnimation::resetToInitialState()
{
}

int DuiAbstractWidgetAnimation::duration() const
{
    return style()->duration();
}

DuiWidgetView *DuiAbstractWidgetAnimation::view()
{
    Q_D(DuiAbstractWidgetAnimation);
    return d->view;
}

const DuiWidgetView *DuiAbstractWidgetAnimation::view() const
{
    Q_D(const DuiAbstractWidgetAnimation);
    return d->view;
}

bool DuiAbstractWidgetAnimation::setView(DuiWidgetView *view)
{
    Q_D(DuiAbstractWidgetAnimation);
    bool inherits = false;
    const QMetaObject *mobj = view->metaObject();
    while (mobj) {
        if (strcmp(viewType(), mobj->className()) == 0) {
            inherits = true;
            break;
        }
        mobj = mobj->superClass();
    }

    if (!inherits)
        return false;

    d->view = view;

    style().setParent(view->d_func()->controller);

    return true;
}
