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

#include "mscenewindowevent_p.h"

static int mSceneWindowAppearEventNumber = -1;
static int mSceneWindowDisappearEventNumber = -1;
static int mSceneWindowDismissEventNumber = -1;

MSceneWindowEvent::MSceneWindowEvent(QEvent::Type type,
        MSceneWindow *sceneWindow,
        bool animatedTransition)
    : QEvent(type),
      m_sceneWindow(sceneWindow),
      m_animatedTransition(animatedTransition)
{
    if (type != eventTypeAppear() && type != eventTypeDisappear() && type != eventTypeDismiss()) {
        qFatal("MSceneWindowEvent: Invalid event type passed on to constructor.");
    }
}

QEvent::Type MSceneWindowEvent::eventTypeAppear()
{
    if (mSceneWindowAppearEventNumber < 0) {
        mSceneWindowAppearEventNumber = QEvent::registerEventType();
    }

    return (QEvent::Type) mSceneWindowAppearEventNumber;
}

QEvent::Type MSceneWindowEvent::eventTypeDisappear()
{
    if (mSceneWindowDisappearEventNumber < 0) {
        mSceneWindowDisappearEventNumber = QEvent::registerEventType();
    }

    return (QEvent::Type) mSceneWindowDisappearEventNumber;
}

QEvent::Type MSceneWindowEvent::eventTypeDismiss()
{
    if (mSceneWindowDismissEventNumber < 0) {
        mSceneWindowDismissEventNumber = QEvent::registerEventType();
    }

    return (QEvent::Type) mSceneWindowDismissEventNumber;
}

MSceneWindow *MSceneWindowEvent::sceneWindow() const
{
    return m_sceneWindow;
}

bool MSceneWindowEvent::animatedTransition() const
{
    return m_animatedTransition;
}
