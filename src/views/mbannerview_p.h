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

#ifndef MINFOBANNERVIEW_P_H
#define MINFOBANNERVIEW_P_H

#include "mscenewindowview_p.h"
#include "mbannerview.h"

class MLabel;
class MLayout;
class MLinearLayoutPolicy;
class MImageWidget;
class MBanner;
class MGridLayoutPolicy;

class MBannerViewPrivate : public MSceneWindowViewPrivate
{
    Q_DECLARE_PUBLIC(MBannerView)

public:

    MBannerViewPrivate();
    virtual ~MBannerViewPrivate();

    void initDynamicLayout();

    MLabel * title();
    MLabel * subtitle();
    MImageWidget * icon();

    void setTitle(const QString& string);
    void setSubtitle(const QString& string);
    void setIcon(const QString& id, const QSize &s);

protected:
    //! The layout of the notification
    MLayout * layout;
    MGridLayoutPolicy *landscapePolicy;
    MGridLayoutPolicy *portraitPolicy;


    MImageWidget *iconId;
    MLabel *titleLabel;
    MLabel *subtitleLabel;

    MBanner *controller;
};

#endif
