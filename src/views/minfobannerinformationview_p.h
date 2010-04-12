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

#ifndef MINFOBANNERINFORMATIONVIEW_P_H
#define MINFOBANNERINFORMATIONVIEW_P_H

class MLabel;
class MImageWidget;
class MButton;
class QGraphicsGridLayout;

class MInfoBannerInformationViewPrivate
{
    Q_DECLARE_PUBLIC(MInfoBannerInformationView)

protected:
    MInfoBannerInformationView *q_ptr;

public:
    MInfoBannerInformationViewPrivate(MInfoBanner *banner);
    virtual ~MInfoBannerInformationViewPrivate();

    virtual void init();
    virtual void setButtonText(const QString &text);

    //! A label for the banner text
    MInfoBanner   *banner;
    MLabel        *label;
    MImageWidget  *image;
    MButton       *button;

    //! The layout of the notification
    QGraphicsGridLayout *layout;

    MInfoBanner *controller;
};

#endif
