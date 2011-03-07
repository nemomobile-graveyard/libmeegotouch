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
#include <QGraphicsLinearLayout>
#include <MLocale>

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

    MLabel *title();
    MLabel *subtitle();
    MImageWidget *icon();
    MLabel *bannerTimeStamp();
    MLabel *prefixTimeStamp();
    MImageWidget *pixmap();

    void setTitle(const QString& string);
    void setSubtitle(const QString& string);
    void setIcon(const QString& id, const QSize &s);
    void setBannerTimeStamp(const QDateTime& date);
    void setPrefixTimeStamp(const QString& string);
    void updateDateFormat() const;
    void refreshStyleMode();
    void manageOpacities() const;
    void setPixmap(const QPixmap& pixmapTemp);

protected:
    MLayout * layout;
    MLinearLayoutPolicy *landscapePolicy;
    MLinearLayoutPolicy *portraitPolicy;
    //For event & lock screen
    QGraphicsGridLayout *gridBanner;

    //Layouts
    MLayout *createLayout();
    QGraphicsGridLayout *createGrid();
    void layoutShortEventBanner();
    void layoutPrivateEventBanner();
    void layoutFullEventBanner();
    void layoutInformationBanner();
    void layoutSystemBanner();
    void layoutGenericBanner();

    MImageWidget *iconId;
    MLabel *titleLabel;
    MLabel *subtitleLabel;
    MLabel *bannerTimeStampLabel;
    MLabel *prefixTimeStampLabel;
    QDateTime *bannerTimeStampData;
    //To activate the opacity in the press down state
    bool isDownOpacityEnabled;
    MImageWidget *pixmapBanner;
    //To perform updateDateFormat only when is needed
    mutable bool timeShortNoDate;

    MBanner *controller;
};

#endif
