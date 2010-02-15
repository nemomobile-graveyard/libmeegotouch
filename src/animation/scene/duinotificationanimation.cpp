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

#include "duinotificationanimation.h"

#if QT_VERSION >= 0x040600

#include "duinotificationanimation_p.h"
#include "duiscenewindow.h"
#include "duiscenemanager.h"
#include "duiapplication.h"
#include "duiwindow.h"
#include <QPropertyAnimation>
#include <QAnimationGroup>

DuiNotificationAnimation::DuiNotificationAnimation(DuiSceneWindow *window, QObject *parent) :
    DuiSceneWindowAnimation(new DuiNotificationAnimationPrivate, window, parent)
{
    Q_D(DuiNotificationAnimation);

    d->positionAnimation = new QPropertyAnimation(window, "pos", group());
}

DuiNotificationAnimation::DuiNotificationAnimation(DuiNotificationAnimationPrivate *dd, DuiSceneWindow *window, QObject *parent) :
    DuiSceneWindowAnimation(dd, window, parent)
{
    Q_D(DuiNotificationAnimation);

    d->positionAnimation = new QPropertyAnimation(window, "pos", group());
}

void DuiNotificationAnimation::setType(DuiSceneWindowAnimation::Type type, bool useCurrentAsStartValue)
{
    Q_D(DuiNotificationAnimation);

    if (useCurrentAsStartValue) {
        d->positionAnimation->setStartValue(d->sceneWindow->pos());
    }

    QSize screenSize = DuiApplication::activeWindow()->visibleSceneSize();

    //TODO: Get the start, end and duration parameters from theme or somewhere.

    switch (type) {
    case DuiSceneWindowAnimation::Show:
        if (!useCurrentAsStartValue) {
            if (d->sceneWindow->windowType() == DuiSceneWindow::NotificationEvent)
                d->positionAnimation->setStartValue(QPointF(-d->sceneWindow->geometry().width(), screenSize.height() * .2));
            else
                d->positionAnimation->setStartValue(QPointF(screenSize.width() + d->sceneWindow->geometry().width(), screenSize.height() * .2));
        }
        if (d->sceneWindow->windowType() == DuiSceneWindow::NotificationEvent)
            d->positionAnimation->setEndValue(QPointF(0, screenSize.height() * .2));
        else
            d->positionAnimation->setEndValue(QPointF(screenSize.width() - d->sceneWindow->geometry().width(), screenSize.height() * .2));
        d->positionAnimation->setDuration(750);
        break;

    case DuiSceneWindowAnimation::Hide:
        if (!useCurrentAsStartValue) {
            if (d->sceneWindow->windowType() == DuiSceneWindow::NotificationEvent)
                d->positionAnimation->setStartValue(QPointF(0, screenSize.height() * .2));
            else
                d->positionAnimation->setStartValue(QPointF(screenSize.width() - d->sceneWindow->geometry().width(), screenSize.height() * .2));
        }
        if (d->sceneWindow->windowType() == DuiSceneWindow::NotificationEvent)
            d->positionAnimation->setEndValue(QPointF(-d->sceneWindow->geometry().width(), screenSize.height() * .2));
        else
            d->positionAnimation->setEndValue(QPointF(screenSize.width() + d->sceneWindow->geometry().width(), screenSize.height() * .2));
        d->positionAnimation->setDuration(750);
        break;

    case DuiSceneWindowAnimation::Move:
        if (!useCurrentAsStartValue) {
            d->positionAnimation->setStartValue(QPointF(0, screenSize.height() * .2));
        }

        d->positionAnimation->setEndValue(QPointF(0, screenSize.height() * .2));
        d->positionAnimation->setDuration(500);
        break;
    }
}
#endif
