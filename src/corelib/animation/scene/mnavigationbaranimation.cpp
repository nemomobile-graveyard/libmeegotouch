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

#include "mnavigationbaranimation.h"

#if QT_VERSION >= 0x040600


#include "mnavigationbaranimation_p.h"
#include "mscenewindow.h"
#include <QPropertyAnimation>
#include <QAnimationGroup>

MNavigationbarAnimation::MNavigationbarAnimation(MSceneWindow *window, QObject *parent) :
    MSceneWindowAnimation(new MNavigationbarAnimationPrivate, window, parent)
{
    Q_D(MNavigationbarAnimation);

    d->positionAnimation = new QPropertyAnimation(window, "pos", group());
    d->opacityAnimation = new QPropertyAnimation(window, "opacity", group());
}

MNavigationbarAnimation::MNavigationbarAnimation(MNavigationbarAnimationPrivate *dd, MSceneWindow *window, QObject *parent) :
    MSceneWindowAnimation(dd, window, parent)
{
    Q_D(MNavigationbarAnimation);

    d->positionAnimation = new QPropertyAnimation(window, "pos", group());
    d->opacityAnimation = new QPropertyAnimation(window, "opacity", group());
}

void MNavigationbarAnimation::setType(MSceneWindowAnimation::Type type, bool useCurrentAsStartValue)
{
    Q_D(MNavigationbarAnimation);

    if (useCurrentAsStartValue) {
        d->positionAnimation->setStartValue(d->sceneWindow->pos());
        d->opacityAnimation->setStartValue(d->sceneWindow->opacity());
    }

    switch (type) {
    case MSceneWindowAnimation::Show:
        if (!useCurrentAsStartValue) {
            d->positionAnimation->setStartValue(QPointF(0, -d->sceneWindow->geometry().height()));
            d->opacityAnimation->setStartValue(0);
        }
        d->positionAnimation->setEndValue(QPointF(0, 0));
        d->opacityAnimation->setEndValue(1);
        d->positionAnimation->setDuration(250);
        d->opacityAnimation->setDuration(250);
        break;

    case MSceneWindowAnimation::Hide:
        if (!useCurrentAsStartValue) {
            d->positionAnimation->setStartValue(QPointF(0, 0));
            d->opacityAnimation->setStartValue(1);
        }
        d->positionAnimation->setEndValue(QPointF(0, -d->sceneWindow->geometry().height()));
        d->opacityAnimation->setEndValue(0);
        d->positionAnimation->setDuration(250);
        d->opacityAnimation->setDuration(250);
        break;

    case MSceneWindowAnimation::Move:
        if (!useCurrentAsStartValue) {
            d->positionAnimation->setStartValue(QPointF(0, 0));
            d->opacityAnimation->setStartValue(1);
        }

        d->positionAnimation->setEndValue(QPointF(0, 0));
        d->opacityAnimation->setEndValue(1);
        d->positionAnimation->setDuration(500);
        d->opacityAnimation->setDuration(500);
        break;
    }
}
#endif
