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

#ifndef UT_MBANNERVIEW_H
#define UT_MBANNERVIEW_H

#include <QObject>

class MBanner;
class MBannerView;
class QGraphicsGridLayout;
class QGraphicsLayoutItem;

class Ut_MBannerView : public QObject
{
    Q_OBJECT

private slots:

    void initTestCase();
    void cleanupTestCase();

    void testInstantiateBannerView();
    void testBannerModelDatainViews();
    void testBannerPixmapAndIcon();
    void testBannerDateFormats();
    void testBannerMouseEvents();
    void testMouseMoveEvent();
    void testDrawForeground();
    void testDrawContents();
    void testBannerStyles();
    void testPrivateManageOpacities();
    void testSettingTimestampDateTimeFormatWhenTimestampForBannerIsSet();
    void testTimestampDateTimeFormatIsUpdatedWhenDayHasPassed();

private:

    void testPrivateLayoutShortEventBanner();
    void testPrivateLayoutInformationBanner();
    void testPrivateLayoutSystemBanner();
    void testPrivateLayoutFullEventBanner();
    void testPrivateLayoutLockScreenEventBanner();
    void testPrivateLayoutPrivateEventBanner();

    void testLayoutSetup();
    void testGridItemsExist();
    void testTitleLabel();
    void testSubtitleLabel();
    void testPixmapOrIconGrid();
    void testPixmapOrIconLayout();
    bool findItemInGridLayout(QGraphicsGridLayout *layout,
                              QGraphicsLayoutItem *item);
    void testLayoutTimeStamp();

    MBanner *m_banner;
    MBannerView *m_subject;
};

#endif
