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

#ifndef UT_DUIAPPLETHANDLEVIEW
#define UT_DUIAPPLETHANDLEVIEW

#include <QObject>
#include "duiapplethandleview.h"
#include "duiapplethandleview_p.h"

class DuiTestAppletHandleView : public DuiAppletHandleView
{
    DUI_VIEW(DuiAppletHandleModel, DuiAppletHandleStyle)

public:
    DuiTestAppletHandleView(DuiAppletHandle *handle);
    void click();
};

class DuiTestAppletHandleViewPrivate : public DuiAppletHandleViewPrivate
{
    Q_DECLARE_PUBLIC(DuiTestAppletHandleView)

public:
    DuiTestAppletHandleViewPrivate(DuiAppletHandle *handle);
    virtual ~DuiTestAppletHandleViewPrivate();

    //! \reimp
    virtual void drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const;
    //! \reimp_end
};

// Test case must inherit QObject
class Ut_DuiAppletHandleView : public QObject
{
    Q_OBJECT

private:
    //! DuiAppletHandleView instance under testing.
    DuiTestAppletHandleView *handleView;
    DuiAppletHandle *handle;

signals:
    void openAppletSettings();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testSettingsDialog();
    void testBrokenAppletDialog();
    void testInstallationFailedDialog();
};
#endif // UT_DUIAPPLETHANDLEVIEW
