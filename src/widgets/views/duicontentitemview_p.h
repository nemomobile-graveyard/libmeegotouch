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

#ifndef DUICONTENTITEM_P_H__
#define DUICONTENTITEM_P_H__

#include "duicontentitemstyle.h"
#include "duicontentitem.h"
#include "private/duiwidgetview_p.h"
#include "duicontentitemstyle.h"

class DuiLabel;
class DuiImageWidget;
class DuiScalableImage;
class QGraphicsGridLayout;

class DuiContentItemViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiContentItemView)

public:
    DuiContentItemViewPrivate();
    virtual ~DuiContentItemViewPrivate();

    inline DuiLabel *title();
    inline DuiLabel *subtitle();
    inline DuiImageWidget *image();

    inline void setTitle(const QString &string);
    inline void setSubtitle(const QString &string);
    inline void setImage(const QPixmap &pixmap);

    void initLayout(DuiContentItem::ContentItemStyle style);
    void clearLayout();

    void applyStyle();

    typedef const DuiScalableImage*(DuiContentItemStyle::*backgroundFunc)() const;

    static void initBackgroundFunctions();
    static QVector<backgroundFunc> backgroundFunctions;

private:
    DuiContentItem *controller;
    DuiLabel *titleLabel;
    DuiLabel *subtitleLabel;
    DuiImageWidget *imageWidget;
    QGraphicsGridLayout *layout;
    int configuredStyle;
    bool down;
};

#endif
