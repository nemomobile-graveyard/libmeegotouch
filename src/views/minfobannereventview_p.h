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

#ifndef MINFOBANNEREVENTVIEW_P_H
#define MINFOBANNEREVENTVIEW_P_H

class MLabel;
class MImageWidget;
class QGraphicsLinearLayout;

class MInfoBannerEventViewPrivate
{
    Q_DECLARE_PUBLIC(MInfoBannerEventView)

protected:
    MInfoBannerEventView *q_ptr;

public:
    MInfoBannerEventViewPrivate(MInfoBanner *controller);
    virtual ~MInfoBannerEventViewPrivate();

    virtual void init();

    //! A label for the notification text
    MInfoBanner   *banner;
    MLabel        *label;
    MImageWidget  *image;
    MImageWidget  *icon;

    //! The layout of the notification
    QGraphicsLinearLayout *layout;

    MInfoBanner *controller;
};

#endif
