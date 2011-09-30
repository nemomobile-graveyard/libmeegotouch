/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#ifndef MTEXTMAGNIFIERSTYLE_H
#define MTEXTMAGNIFIERSTYLE_H

#include <MWidgetStyle>
#include <QEasingCurve>

class QString;
class MWidgetController;

class MTextMagnifierStyle
{
public:
    qreal magnification() const { return 0.0; }
    QPixmap *magnifierMask() const { return 0; }
    QPixmap *magnifierFrame() const { return 0; }
    QPointF visualOffset() const { return QPointF(); }
    int appearanceDuration() const { return 0; }
    QEasingCurve appearanceEasingCurve() const { return QEasingCurve(); }

}static gMagnifierStyle;

class MTextMagnifierStyleContainer : public MWidgetStyleContainer
{
public:
    void initialize(const QString &, const QString &, const MWidgetController *){}
    void setModeDefault() const {}
    const MTextMagnifierStyle* operator->() const
    {
        return &gMagnifierStyle;
    }
};


#endif // MTEXTMAGNIFIERSTYLE_H
