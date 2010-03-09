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

#include "duikeyboardstatetracker.h"
#include "duikeyboardstatetracker_p.h"
#include <QVariant>
#include <QCoreApplication>

#include <DuiDebug>

DuiKeyboardStateTracker *DuiKeyboardStateTrackerPrivate::tracker = 0;

DuiKeyboardStateTrackerPrivate::DuiKeyboardStateTrackerPrivate(DuiKeyboardStateTracker *controller) :
#ifdef HAVE_CONTEXTSUBSCRIBER
    keyboardOpenProperty("/maemo/InternalKeyboard/Open"),
#endif
    q_ptr(controller),
    present(false)
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_Q(DuiKeyboardStateTracker);
    connect(&keyboardOpenProperty, SIGNAL(valueChanged()),
            q, SIGNAL(stateChanged()));
#endif

    initContextSubscriber();
}

void DuiKeyboardStateTrackerPrivate::initContextSubscriber()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    //waiting for properties to synchronize
    keyboardOpenProperty.waitForSubscription();
    // TODO: use actual ContextProperty for present, which is still unready.
    present = true;
#endif
}

DuiKeyboardStateTracker::DuiKeyboardStateTracker() :
    d_ptr(new DuiKeyboardStateTrackerPrivate(this))
{
    setParent(QCoreApplication::instance()); //get collected when needed.
}

DuiKeyboardStateTracker *DuiKeyboardStateTracker::instance()
{
    if (!DuiKeyboardStateTrackerPrivate::tracker)
        DuiKeyboardStateTrackerPrivate::tracker = new DuiKeyboardStateTracker();
    return DuiKeyboardStateTrackerPrivate::tracker;
}

DuiKeyboardStateTracker::~DuiKeyboardStateTracker()
{
    if (this == DuiKeyboardStateTrackerPrivate::tracker)
        DuiKeyboardStateTrackerPrivate::tracker = 0;
}

bool DuiKeyboardStateTracker::isPresent() const
{
    Q_D(const DuiKeyboardStateTracker);

    return d->present;
}

bool DuiKeyboardStateTracker::isOpen() const
{
    bool val = false;
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_D(const DuiKeyboardStateTracker);
    if (isPresent()) {
        val = d->keyboardOpenProperty.value().toBool();
    }
#endif
    return val;
}
