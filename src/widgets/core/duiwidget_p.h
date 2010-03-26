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

#ifndef DUIWIDGET_P_H
#define DUIWIDGET_P_H

#include <duiwidget.h>

class DuiWidgetPrivate
{
    Q_DECLARE_PUBLIC(DuiWidget)
public:
    DuiWidgetPrivate();
    virtual ~DuiWidgetPrivate();

    void doEnterDisplayEvent();
    void doExitDisplayEvent();

    void sendOnDisplayChangeEvent(DuiWidget *widget, const QRectF *visibleSceneRect);
    void resolveIsOnDisplay(QGraphicsItem *item, const QRectF *visibleSceneRect);
    QGraphicsView *fetchGraphicsView();

    bool explicitlyHidden;  //< true if the user called hide(), as opposed to DuiLayout hiding it
    bool layoutHidden;  //< true if DuiLayout is currently hiding the widget

protected:
    DuiWidget *q_ptr;
    bool onDisplay;
    bool onDisplaySet;
    bool selected;
};

#endif // DUIWIDGET_P_H
