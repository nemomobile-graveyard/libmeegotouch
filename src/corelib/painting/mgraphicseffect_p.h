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

#ifndef MGRAPHICSEFFECTGROUP_P_H
#define MGRAPHICSEFFECTGROUP_P_H

#include <QPointer>
#include <QPropertyAnimation>

class MGraphicsEffectStyleContainer;

class MGraphicsEffectPrivate
{
    Q_DECLARE_PUBLIC(MGraphicsEffect)
public:
    MGraphicsEffectPrivate();
    virtual ~MGraphicsEffectPrivate() {}

    void setLevel(int newLevel);
    void setIntensity(qreal newIntensity);

protected:
    class MGraphicsEffect *q_ptr;

private:
    MGraphicsEffectStyleContainer *styleContainer;

    int level;
    qreal intensity;

    int previousLevel;
    QPointer<QPropertyAnimation> animation;
};

#endif
