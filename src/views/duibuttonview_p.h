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

#ifndef DUIBUTTONVIEW_P_H
#define DUIBUTTONVIEW_P_H

#include "private/duiwidgetview_p.h"

class QPixmap;
class DuiScalableImage;
class DuiLabel;

class DuiButtonViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiButtonView)

public:
    DuiButtonViewPrivate();
    virtual ~DuiButtonViewPrivate();

    const QPixmap *icon;
    const QPixmap *toggledIcon;

    DuiLabel *label;

    QRectF iconRect;

    void calcIconTextRects();
    bool toggleState() const;
    void refreshStyleMode();

    void loadIcon(const QPixmap*& icon, const QString &newIconId, const QSize &newIconSize);
};

#endif
