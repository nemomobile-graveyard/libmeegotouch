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

#include "mscenefadeanimation.h"

#if QT_VERSION >= 0x040600


#include "mscenefadeanimation_p.h"
#include "mscenewindow.h"
#include "mscenemanager.h"
#include "mwidgetanimation.h"

//#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QEasingCurve>

MSceneFadeAnimation::MSceneFadeAnimation(MSceneWindow *window, QObject *parent) :
    MSceneWindowAnimation(new MSceneFadeAnimationPrivate, window, parent)
{
    Q_D(MSceneFadeAnimation);

    //d->positionAnimation = new QPropertyAnimation(window, "pos", group());
    d->animation = new MWidgetAnimation(group());
    //d->animation->addWidget(window);
}

MSceneFadeAnimation::MSceneFadeAnimation(MSceneFadeAnimationPrivate *dd, MSceneWindow *window, QObject *parent) :
    MSceneWindowAnimation(dd, window, parent)
{
    Q_D(MSceneFadeAnimation);

    //d->positionAnimation = new QPropertyAnimation(window, "pos", group());
    d->animation = new MWidgetAnimation(group());
    d->animation->addWidget(window);
}

void MSceneFadeAnimation::setType(MSceneWindowAnimation::Type type, bool useCurrentAsStartValue)
{
    Q_D(MSceneFadeAnimation);

    switch (type) {
    case MSceneWindowAnimation::Show: {

        if (!useCurrentAsStartValue)
            d->sceneWindow->setOpacity(0.0);

        /*d->sceneWindow->setTransformOrigin(QPointF(432, 240));
        d->sceneWindow->setXScale(0.0);
        d->sceneWindow->setYScale(0.0);
        d->animation->setTargetValue(d->sceneWindow, "xScale",1.0);
        d->animation->setTargetValue(d->sceneWindow, "yScale",1.0);
        d->animation->setDuration(500);*/
        //d->animation->setEasingCurve(QEasingCurve::OutBounce);

        d->animation->setTargetOpacity(d->sceneWindow, 1.0);
        d->animation->setDuration(style()->showDuration(), "opacity");
    } break;

    case MSceneWindowAnimation::Hide:

        d->animation->setTargetOpacity(d->sceneWindow, 0.0);
        d->animation->setDuration(style()->hideDuration());
        break;

    case MSceneWindowAnimation::Move:
        break;
    }
}
#endif
