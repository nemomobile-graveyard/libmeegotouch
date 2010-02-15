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

#ifndef DUISPINNERVIEW_P_H
#define DUISPINNERVIEW_P_H

#include <QObject>
#include <QPointF>
#include "private/duiwidgetview_p.h"

class DuiStyle;
class DuiProgressIndicator;
class QPixmap;
class QTimer;

class DuiSpinnerViewPrivate : public DuiWidgetViewPrivate
{
public:
    Q_DECLARE_PUBLIC(DuiSpinnerView)

    DuiSpinnerViewPrivate();
    virtual ~DuiSpinnerViewPrivate();

    void animationTimeout();
    void visibilityChanged(bool);
    void calculateShape(QSizeF size);

    DuiProgressIndicator *controller;

    const QPixmap *inactiveElement;
    const QPixmap *activeElement;
    int position;
    qreal elapsed;

    QTimer *timer;
    QVector<QPoint> elements;

#ifdef DUI_UNIT_TEST
    DUI_UNIT_TEST;
#endif
};

#endif // DUISPINNERVIEW_P_H
