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

#include "mkeyboardstatetracker.h"
#include "mkeyboardstatetracker_p.h"
#include <QVariant>
#include <QCoreApplication>

#include <MDebug>

MKeyboardStateTracker *MKeyboardStateTrackerPrivate::tracker = 0;

MKeyboardStateTrackerPrivate::MKeyboardStateTrackerPrivate(MKeyboardStateTracker *controller) :
#ifdef HAVE_CONTEXTSUBSCRIBER
    keyboardOpenProperty("/maemo/InternalKeyboard/Open"),
#elif defined(HAVE_N900)
    keyboardOpenConf("/system/osso/af/slide-open"),
#endif
    q_ptr(controller),
    present(false)
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_Q(MKeyboardStateTracker);
    QObject::connect(&keyboardOpenProperty, SIGNAL(valueChanged()),
                     q, SIGNAL(stateChanged()));
#elif defined(HAVE_N900)
    Q_Q(MKeyboardStateTracker);
    QObject::connect(&keyboardOpenConf, SIGNAL(valueChanged()),
                     q, SIGNAL(stateChanged()));
#endif

    initContextSubscriber();
}

void MKeyboardStateTrackerPrivate::initContextSubscriber()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    //waiting for properties to synchronize
    keyboardOpenProperty.waitForSubscription();
    // TODO: use actual ContextProperty for present, which is still unready.
    present = true;
#elif defined(HAVE_N900)
    present = true;
#endif
}

MKeyboardStateTracker::MKeyboardStateTracker() :
    d_ptr(new MKeyboardStateTrackerPrivate(this))
{
    setParent(QCoreApplication::instance()); //get collected when needed.
}

MKeyboardStateTracker *MKeyboardStateTracker::instance()
{
    if (!MKeyboardStateTrackerPrivate::tracker)
        MKeyboardStateTrackerPrivate::tracker = new MKeyboardStateTracker();
    return MKeyboardStateTrackerPrivate::tracker;
}

MKeyboardStateTracker::~MKeyboardStateTracker()
{
    if (this == MKeyboardStateTrackerPrivate::tracker)
        MKeyboardStateTrackerPrivate::tracker = 0;
}

bool MKeyboardStateTracker::isPresent() const
{
    Q_D(const MKeyboardStateTracker);

    return d->present;
}

bool MKeyboardStateTracker::isOpen() const
{
    bool val = false;
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_D(const MKeyboardStateTracker);
    if (isPresent()) {
        val = d->keyboardOpenProperty.value().toBool();
    }
#elif defined(HAVE_N900)
    Q_D(const MKeyboardStateTracker);
    val = d->keyboardOpenConf.value().toBool();
#endif
    return val;
}
