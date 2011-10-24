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
#ifndef MTEXTSELECTIONHANDLESTYLE_H
#define MTEXTSELECTIONHANDLESTYLE_H

#include <MWidgetStyle>
#include <mfeedback.h>
#include <QEasingCurve>

class QString;
class MWidgetController;

class MTextSelectionHandleStyle
{
public:
    qreal visualOffset() const { return 0.0; }
    int showHideDuration() const { return 50; }
    int movementSensitivity() const { return 0; }
    int disableDuration() const { return 0; }
    qreal disableOpacity() const { return 0.5; }
    QEasingCurve showHideEasingCurve() const { return QEasingCurve(); }

    MFeedback pressFeedback() const { return MFeedback(); }
    MFeedback releaseFeedback() const { return MFeedback(); }

    QSizeF preferredSize() const { return QSizeF(30, 80); }

}static gTextSelectionHandleStyle;

class MTextSelectionHandleStyleContainer : public MWidgetStyleContainer
{
public:
    void initialize(const QString &, const QString &, const MWidgetController *){}
    void setModeDefault() const {}
    const MTextSelectionHandleStyle* operator->() const
    {
        return &gTextSelectionHandleStyle;
    }
};


#endif // MTEXTSELECTIONHANDLESTYLE_H
