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

#include "duiscenewindowevent_p.h"

static int duiSceneWindowAppearEventNumber = -1;
static int duiSceneWindowDisappearEventNumber = -1;
static int duiSceneWindowDismissEventNumber = -1;

DuiSceneWindowEvent::DuiSceneWindowEvent(QEvent::Type type,
        DuiSceneWindow *sceneWindow,
        bool animatedTransition)
    : QEvent(type),
      m_sceneWindow(sceneWindow),
      m_animatedTransition(animatedTransition)
{
    if (type != eventTypeAppear() && type != eventTypeDisappear() && type != eventTypeDismiss()) {
        qFatal("DuiSceneWindowEvent: Invalid event type passed on to constructor.");
    }
}

QEvent::Type DuiSceneWindowEvent::eventTypeAppear()
{
    if (duiSceneWindowAppearEventNumber < 0) {
        duiSceneWindowAppearEventNumber = QEvent::registerEventType();
    }

    return (QEvent::Type) duiSceneWindowAppearEventNumber;
}

QEvent::Type DuiSceneWindowEvent::eventTypeDisappear()
{
    if (duiSceneWindowDisappearEventNumber < 0) {
        duiSceneWindowDisappearEventNumber = QEvent::registerEventType();
    }

    return (QEvent::Type) duiSceneWindowDisappearEventNumber;
}

QEvent::Type DuiSceneWindowEvent::eventTypeDismiss()
{
    if (duiSceneWindowDismissEventNumber < 0) {
        duiSceneWindowDismissEventNumber = QEvent::registerEventType();
    }

    return (QEvent::Type) duiSceneWindowDismissEventNumber;
}

DuiSceneWindow *DuiSceneWindowEvent::sceneWindow() const
{
    return m_sceneWindow;
}

bool DuiSceneWindowEvent::animatedTransition() const
{
    return m_animatedTransition;
}
