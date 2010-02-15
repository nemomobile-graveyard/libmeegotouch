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

#ifndef DUIINFOBANNERINFORMATIONVIEW_P_H
#define DUIINFOBANNERINFORMATIONVIEW_P_H

#include "duiscenewindowview_p.h"

class DuiLabel;
class DuiImageWidget;
class DuiButton;
class QGraphicsGridLayout;

class DuiInfoBannerInformationViewPrivate : public DuiSceneWindowViewPrivate
{
    Q_DECLARE_PUBLIC(DuiInfoBannerInformationView)

public:
    DuiInfoBannerInformationViewPrivate(DuiInfoBanner *banner);
    virtual ~DuiInfoBannerInformationViewPrivate();

    virtual void init();
    virtual void setButtonText(const QString &text);

    //! A label for the banner text
    DuiInfoBanner   *banner;
    DuiLabel        *label;
    DuiImageWidget  *image;
    DuiButton       *button;

    //! The layout of the notification
    QGraphicsGridLayout *layout;
};

#endif
