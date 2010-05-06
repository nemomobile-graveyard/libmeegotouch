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

#ifndef MWIDGETANIMATION_P_H
#define MWIDGETANIMATION_P_H

#include "mgroupanimation_p.h"

#include <QEasingCurve>
#include <QMap>

class QPropertyAnimation;

typedef QMap<QString, QPropertyAnimation *> Animations;

class MWidgetAnimationPrivate : public MGroupAnimationPrivate
{
    Q_DECLARE_PUBLIC(MWidgetAnimation)

public:

    MWidgetAnimationPrivate();
    virtual ~MWidgetAnimationPrivate();

    QPropertyAnimation *getAnimation(QGraphicsWidget *widget, const QString &property);

    void setTargetForAllWidgets(const QString &property, const QVariant &value);

    QMap<QGraphicsWidget *, Animations> animations;
    int duration;
    QEasingCurve easingCurve;
};

#endif
