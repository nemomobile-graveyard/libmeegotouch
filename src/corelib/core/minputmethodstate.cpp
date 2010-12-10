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

#include "minputmethodstate.h"
#include "minputmethodstate_p.h"

#include "qapplication.h"
#include "qinputcontext.h"

#include <QTimer>
#include <QKeyEvent>


MInputMethodStatePrivate::MInputMethodStatePrivate()
    : orientation(M::Angle0),
      q_ptr(NULL)
{
}

MInputMethodStatePrivate::~MInputMethodStatePrivate()
{
}

void MInputMethodStatePrivate::init()
{
}

MInputMethodState::MInputMethodState()
    : d_ptr(new MInputMethodStatePrivate)
{
    Q_D(MInputMethodState);

    d->q_ptr = this;
    d->init();
}

MInputMethodState::~MInputMethodState()
{
    delete d_ptr;
}

MInputMethodState *MInputMethodState::instance()
{
    static MInputMethodState singleton;

    return &singleton;
}

void MInputMethodState::setInputMethodArea(const QRect &newRegion)
{
    Q_D(MInputMethodState);

    if (d->region != newRegion) {
        d->region = newRegion;
        emit inputMethodAreaChanged(d->region);
    }
}

QRect MInputMethodState::inputMethodArea() const
{
    Q_D(const MInputMethodState);

    return d->region;
}

void MInputMethodState::setActiveWindowOrientationAngle(M::OrientationAngle newOrientation)
{
    Q_D(MInputMethodState);

    if (d->orientation != newOrientation) {
        d->orientation = newOrientation;
        emit activeWindowOrientationAngleChanged(newOrientation);
    }
}

M::OrientationAngle MInputMethodState::activeWindowOrientationAngle() const
{
    Q_D(const MInputMethodState);

    return d->orientation;
}

void MInputMethodState::requestSoftwareInputPanel()
{
    QInputContext *inputContext = qApp->inputContext();

    if (!inputContext) {
        return;
    }

    QEvent request(QEvent::RequestSoftwareInputPanel);
    inputContext->filterEvent(&request);
}

void MInputMethodState::closeSoftwareInputPanel()
{
    QInputContext *inputContext = qApp->inputContext();

    if (!inputContext) {
        return;
    }

    QEvent close(QEvent::CloseSoftwareInputPanel);
    inputContext->filterEvent(&close);
    inputContext->reset();
}

void MInputMethodState::emitKeyPress(const QKeyEvent &event)
{
    emit keyPress(event);
}

void MInputMethodState::emitKeyRelease(const QKeyEvent &event)
{
    emit keyRelease(event);
}

QList<int> MInputMethodState::toolbarIds() const
{
    Q_D(const MInputMethodState);
    return d->toolbars.keys();
}

MInputMethodState::ItemAttributeMap MInputMethodState::toolbarState(int id) const
{
    Q_D(const MInputMethodState);
    ToolbarInfo *toolbar = d->toolbars.value(id);

    return (toolbar ? toolbar->items : ItemAttributeMap());
}

int MInputMethodState::registerToolbar(const QString &fileName)
{
    Q_D(MInputMethodState);
    static int idCounter = 0;
    // generate an application local unique identifier for the toolbar.
    int newId = idCounter;
    idCounter++;
    d->toolbars.insert(newId, new ToolbarInfo(fileName));
    emit toolbarRegistered(newId, fileName);
    return newId;
}

void MInputMethodState::unregisterToolbar(int id)
{
    Q_D(MInputMethodState);
    delete d->toolbars[id];
    d->toolbars.remove(id);
    emit toolbarUnregistered(id);
}

void MInputMethodState::setToolbarItemAttribute(int id, const QString &item,
                                                const QString &attribute, const QVariant &value)
{
    Q_D(MInputMethodState);

    if (!d->toolbars.contains(id)) {
        return;
    }

    ToolbarInfo *toolbar = d->toolbars[id];
    bool changed = (value != toolbar->items[item][attribute]);

    if (changed) {
        toolbar->items[item][attribute] = value;
        emit toolbarItemAttributeChanged(id, item, attribute, value);
    }
}

QString MInputMethodState::toolbar(int id) const
{
    Q_D(const MInputMethodState);

    ToolbarInfo *toolbar = d->toolbars.value(id);

    if (toolbar) {
        return toolbar->fileName;
    } else {
        return QString();
    }
}

#include "moc_minputmethodstate.cpp"
