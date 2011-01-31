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

#ifndef MPOSITIONINDICATORVIEW_P_H
#define MPOSITIONINDICATORVIEW_P_H

#include "private/mwidgetview_p.h"

class MPositionIndicator;
class QPixmap;
class QPoint;
class QPropertyAnimation;

class MPositionIndicatorViewPrivate : public MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MPositionIndicatorView)

public:
    MPositionIndicatorViewPrivate();
    void init(MPositionIndicator *controller);
    virtual ~MPositionIndicatorViewPrivate();

    /**
     * Returns true if the position indicator is part of a minimized
     * window in the switcher.
     */
    bool isInSwitcher() const;

    /**
     * Returns true if the content the position-indicator refers to is
     * fully visible.
     */
    bool contentFullyVisible() const;

    MPositionIndicator *controller;
    QTimer *hideTimer;
    bool visible;
    bool onDisplay;

    QPropertyAnimation *fadeAnimation;

    void _q_displayEntered();
    void _q_displayExited();
};

#endif
