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

#ifndef MBUTTONVIEW_P_H
#define MBUTTONVIEW_P_H

#include "private/mwidgetview_p.h"

class QPixmap;
class MScalableImage;
class MLabel;

class MButtonViewPrivate : public MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MButtonView)

public:
    MButtonViewPrivate();
    virtual ~MButtonViewPrivate();

    const QPixmap *icon;
    const QPixmap *toggledIcon;

    MLabel *label;

    QRectF iconRect;

    void calcIconTextRects();
    bool toggleState() const;
    void refreshStyleMode();

    void loadIcon(const QPixmap*& icon, const QString &newIconId, const QSize &newIconSize);
};

#endif
