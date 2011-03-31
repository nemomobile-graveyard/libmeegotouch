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
#include <QIcon>

class MScalableImage;
class MLabel;
class QTimer;
class MButtonTransition;

class MButtonViewPrivate : public MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MButtonView)

public:
    enum IconOrigin {
        IconOriginUndefined,
        IconOriginFromModelIconId,
        IconOriginFromModelQIcon,
        IconOriginFromStyleIconId
    };

    class Icon {
        public:
        Icon();
        virtual ~Icon();

        const QPixmap *pixmap;
        IconOrigin origin; // from where the pixmap was taken from

        // OBS: Those are not used when origin is QIcon
        QString id; // id used to fetch the pixmap
        QString theme; // current theme when time the pixmap was fetched
    };

    MButtonViewPrivate();
    virtual ~MButtonViewPrivate();

    void freeIcons();

    void updateIcon(Icon **icon,
            QIcon::Mode mode, const QString &iconIdFromModel, const QString &iconIdFromStyle);

    bool isCurrentIconObsolete(const QString &newIconId) const;

    void updateIcon();
    void updateToggledIcon();

    void setText(const QString &text);
    virtual void updateLabelStyle();

    // The icons come either from the style or from the model.
    Icon *icon;
    Icon *toggledIcon;

    MLabel *label;

    IconOrigin iconOrigin;
    IconOrigin toggledIconOrigin;

    QRectF iconRect;

    void calcIconTextRects();
    bool toggleState() const;

    void loadIcon(const QIcon &qIcon, const QSize &newIconSize);
    void loadIcon(const QString &newIconId, const QSize &newIconSize, QIcon::Mode mode = QIcon::Normal);

    virtual void updateItemsAfterModeChange();

    int pressTimeout() const;
    MButtonTransition* transition;

    bool eventCancelled;
    bool expectMouseReleaseEvent;
};

#endif
