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
#include <QGraphicsGridLayout>

class MLabel;
class MLayout;
class MLinearLayoutPolicy;
class MImageWidget;
class MBanner;
class MGridLayoutPolicy;
class QDateTime;

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
    MLabel *bannerTimeStamp();

    void setTitle(const QString& string);
    void setSubtitle(const QString& string);
    void setIcon(const QString& id, const QSize &s);
    void setBannerTimeStamp(const QDateTime& date);

protected:

    int MBannerStyle;

    MLayout * layout;
    MLinearLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;
    //For event & lock screen
    QGraphicsGridLayout *gridBanner;

    //Layouts
    MLayout *createLayout();
    QGraphicsGridLayout *createGrid();
    void layoutEventBanner();
    void layoutInformationBanner();
    void layoutSystemBanner();
    void layoutEventScreen();
    void layoutEventLockScreen();
    void layoutGenericBanner();

    MImageWidget *iconId;
    MLabel *titleLabel;
    MLabel *subtitleLabel;
    MLabel *bannerTimeStampLabel;
    QDateTime *bannerTimeStampData;

    MBanner *controller;
};

#endif
