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

#include "mnotificationanimation.h"

#if QT_VERSION >= 0x040600

#include "mnotificationanimation_p.h"
#include "mscenewindow.h"
#include "mscenemanager.h"
#include "mapplication.h"
#include "mwindow.h"
#include <QPropertyAnimation>
#include <QAnimationGroup>

MNotificationAnimation::MNotificationAnimation(MSceneWindow *window, QObject *parent) :
    MSceneWindowAnimation(new MNotificationAnimationPrivate, window, parent)
{
    Q_D(MNotificationAnimation);

    d->positionAnimation = new QPropertyAnimation(window, "pos", group());
}

MNotificationAnimation::MNotificationAnimation(MNotificationAnimationPrivate *dd, MSceneWindow *window, QObject *parent) :
    MSceneWindowAnimation(dd, window, parent)
{
    Q_D(MNotificationAnimation);

    d->positionAnimation = new QPropertyAnimation(window, "pos", group());
}

void MNotificationAnimation::setType(MSceneWindowAnimation::Type type, bool useCurrentAsStartValue)
{
    Q_D(MNotificationAnimation);

    if (useCurrentAsStartValue) {
        d->positionAnimation->setStartValue(d->sceneWindow->pos());
    }

    QSize screenSize = MApplication::activeWindow()->visibleSceneSize();

    //TODO: Get the start, end and duration parameters from theme or somewhere.

    switch (type) {
    case MSceneWindowAnimation::Show:
        if (!useCurrentAsStartValue) {
            if (d->sceneWindow->windowType() == MSceneWindow::NotificationEvent)
                d->positionAnimation->setStartValue(QPointF(-d->sceneWindow->geometry().width(), screenSize.height() * .2));
            else
                d->positionAnimation->setStartValue(QPointF(screenSize.width() + d->sceneWindow->geometry().width(), screenSize.height() * .2));
        }
        if (d->sceneWindow->windowType() == MSceneWindow::NotificationEvent)
            d->positionAnimation->setEndValue(QPointF(0, screenSize.height() * .2));
        else
            d->positionAnimation->setEndValue(QPointF(screenSize.width() - d->sceneWindow->geometry().width(), screenSize.height() * .2));
        d->positionAnimation->setDuration(750);
        break;

    case MSceneWindowAnimation::Hide:
        if (!useCurrentAsStartValue) {
            if (d->sceneWindow->windowType() == MSceneWindow::NotificationEvent)
                d->positionAnimation->setStartValue(QPointF(0, screenSize.height() * .2));
            else
                d->positionAnimation->setStartValue(QPointF(screenSize.width() - d->sceneWindow->geometry().width(), screenSize.height() * .2));
        }
        if (d->sceneWindow->windowType() == MSceneWindow::NotificationEvent)
            d->positionAnimation->setEndValue(QPointF(-d->sceneWindow->geometry().width(), screenSize.height() * .2));
        else
            d->positionAnimation->setEndValue(QPointF(screenSize.width() + d->sceneWindow->geometry().width(), screenSize.height() * .2));
        d->positionAnimation->setDuration(750);
        break;

    case MSceneWindowAnimation::Move:
        if (!useCurrentAsStartValue) {
            d->positionAnimation->setStartValue(QPointF(0, screenSize.height() * .2));
        }

        d->positionAnimation->setEndValue(QPointF(0, screenSize.height() * .2));
        d->positionAnimation->setDuration(500);
        break;
    }
}
#endif
