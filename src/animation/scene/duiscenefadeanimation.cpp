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

#include "duiscenefadeanimation.h"

#if QT_VERSION >= 0x040600


#include "duiscenefadeanimation_p.h"
#include "duiscenewindow.h"
#include "duiscenemanager.h"
#include "duiwidgetanimation.h"

//#include <QPropertyAnimation>
#include <QAnimationGroup>
#include <QEasingCurve>

DuiSceneFadeAnimation::DuiSceneFadeAnimation(DuiSceneWindow *window, QObject *parent) :
    DuiSceneWindowAnimation(new DuiSceneFadeAnimationPrivate, window, parent)
{
    Q_D(DuiSceneFadeAnimation);

    //d->positionAnimation = new QPropertyAnimation(window, "pos", group());
    d->animation = new DuiWidgetAnimation(group());
    //d->animation->addWidget(window);
}

DuiSceneFadeAnimation::DuiSceneFadeAnimation(DuiSceneFadeAnimationPrivate *dd, DuiSceneWindow *window, QObject *parent) :
    DuiSceneWindowAnimation(dd, window, parent)
{
    Q_D(DuiSceneFadeAnimation);

    //d->positionAnimation = new QPropertyAnimation(window, "pos", group());
    d->animation = new DuiWidgetAnimation(group());
    d->animation->addWidget(window);
}

void DuiSceneFadeAnimation::setType(DuiSceneWindowAnimation::Type type, bool useCurrentAsStartValue)
{
    Q_D(DuiSceneFadeAnimation);

    switch (type) {
    case DuiSceneWindowAnimation::Show: {

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

    case DuiSceneWindowAnimation::Hide:

        d->animation->setTargetOpacity(d->sceneWindow, 0.0);
        d->animation->setDuration(style()->hideDuration());
        break;

    case DuiSceneWindowAnimation::Move:
        break;
    }
}
#endif
