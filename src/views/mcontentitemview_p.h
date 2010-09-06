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

#ifndef MCONTENTITEM_P_H__
#define MCONTENTITEM_P_H__

#include "mcontentitemstyle.h"
#include "mcontentitem.h"
#include "private/mwidgetview_p.h"
#include "mcontentitemstyle.h"

class MLabel;
class MImageWidget;
class MScalableImage;
class QGraphicsGridLayout;
class QTimer;

class MContentItemViewPrivate : public MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MContentItemView)

public:
    MContentItemViewPrivate();
    virtual ~MContentItemViewPrivate();

    inline MLabel * title();
    inline MLabel * subtitle();
    inline MImageWidget * image();
    inline MImageWidget * optionalImage();

    inline void setTitle(const QString& string);
    inline void setSubtitle(const QString& string);
    inline void setPixmap(const QPixmap& pixmap);
    inline void setOptionalPixmap(const QPixmap& pixmap);
    inline void setImage(const QImage& image);
    inline void setImage(const QString& id, const QSize &s);
    inline void setOptionalImage(const QImage& image);
    inline void setOptionalImage(const QString& id, const QSize &s);

    QSize preferredImageSize(MImageWidget *image);

    void initLayout(MContentItem::ContentItemStyle style);
    void clearLayout();
    void initTwoIconsTwoWidgetsLayout();

    void applyStyle();
    void refreshStyleMode();

    void longTap(const QPointF &pos);

    typedef const MScalableImage*(MContentItemStyle::*backgroundFunc)() const;

    static void initBackgroundFunctions();
    static QVector<backgroundFunc> backgroundFunctions;

    void _q_applyQueuedStyleModeChange();
private:
    MContentItem *controller;
    MLabel *titleLabel;
    MLabel *subtitleLabel;
    MImageWidget *imageWidget;
    QGraphicsGridLayout *layout;
    int configuredStyle;
    bool down;
    MImageWidget * optionalImageWidget;
    QTimer *styleModeChangeTimer;
    bool queuedStyleModeChange;
};

#endif
