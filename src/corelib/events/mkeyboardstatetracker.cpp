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

#ifdef HAVE_CONTEXTSUBSCRIBER
#include "contextproperty.h"
#endif

#include <MDebug>

namespace {
    const QString keyboardPresent("/maemo/InternalKeyboard/Present");
}

MKeyboardStateTracker *MKeyboardStateTrackerPrivate::tracker = 0;

MKeyboardStateTrackerPrivate::MKeyboardStateTrackerPrivate(MKeyboardStateTracker *controller) :
#ifdef HAVE_CONTEXTSUBSCRIBER
    keyboardOpenProperty(0),
    isSubscribed(false),
#elif defined(M_OS_MAEMO5)
    keyboardOpenConf("/system/osso/af/slide-open"),
#endif
    q_ptr(controller),
    present(false)
{
    Q_Q(MKeyboardStateTracker);
#ifdef HAVE_CONTEXTSUBSCRIBER
    checkKeyboardPresent();
    if (present) {
        keyboardOpenProperty = new ContextProperty(QString("/maemo/InternalKeyboard/Open"));
        QObject::connect(keyboardOpenProperty, SIGNAL(valueChanged()),
                     q, SIGNAL(stateChanged()));
    }
#elif defined(M_OS_MAEMO5)
    QObject::connect(&keyboardOpenConf, SIGNAL(valueChanged()),
                     q, SIGNAL(stateChanged()));
#else
    Q_UNUSED(q);
#endif

    initContextSubscriber();
}

MKeyboardStateTrackerPrivate::~MKeyboardStateTrackerPrivate()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    delete keyboardOpenProperty;
#endif
}

void MKeyboardStateTrackerPrivate::initContextSubscriber()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (!present)
        return;
    //waiting for properties to synchronize
    keyboardOpenProperty->waitForSubscription(true);
    isSubscribed = true;
#elif defined(M_OS_MAEMO5)
    present = true;
#endif
}

#ifdef HAVE_CONTEXTSUBSCRIBER
void MKeyboardStateTrackerPrivate::checkKeyboardPresent()
{
    ContextProperty keyboardPresentProperty(keyboardPresent);
    keyboardPresentProperty.waitForSubscription(true);
    present = keyboardPresentProperty.value().toBool();
}
#endif

void MKeyboardStateTrackerPrivate::subscribe()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (present)
        keyboardOpenProperty->subscribe();
#endif
}

void MKeyboardStateTrackerPrivate::unsubscribe()
{
#ifdef HAVE_CONTEXTSUBSCRIBER
    if (present) {
        keyboardOpenProperty->unsubscribe();
        isSubscribed = false;
    }
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
    delete d_ptr;
}

bool MKeyboardStateTracker::isPresent() const
{
    Q_D(const MKeyboardStateTracker);

    return d->present;
}

bool MKeyboardStateTracker::isOpen() const
{
    bool val = false;
    Q_D(const MKeyboardStateTracker);
    if (d->present) {
#ifdef HAVE_CONTEXTSUBSCRIBER
        MKeyboardStateTrackerPrivate* trackerPriv = const_cast<MKeyboardStateTrackerPrivate*>(d);
        if (!d->isSubscribed) {
            trackerPriv->subscribe();
            trackerPriv->initContextSubscriber();
        }
        val = d->keyboardOpenProperty->value().toBool();
#elif defined(M_OS_MAEMO5)
        val = d->keyboardOpenConf.value().toBool();
#endif
    }
    return val;
}
