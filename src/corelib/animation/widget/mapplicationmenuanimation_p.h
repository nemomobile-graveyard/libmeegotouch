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

#ifndef MAPPLICATIONMENUANIMATION_P_H
#define MAPPLICATIONMENUANIMATION_P_H

#include "mdialoganimation_p.h"
#include "mapplicationmenuanimation.h"

class MApplicationMenuAnimationPrivate : public MDialogAnimationPrivate
{
    Q_DECLARE_PUBLIC(MApplicationMenuAnimation)

public:
    MApplicationMenuAnimationPrivate();

    virtual void setupDurations();
    virtual void setupEasingCurves();
    virtual QPointF setupPositionAnimation(const QPointF &widgetPos);
};

#endif // MAPPLICATIONMENUANIMATION_P_H
