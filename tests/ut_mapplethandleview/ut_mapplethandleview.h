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

#ifndef UT_MAPPLETHANDLEVIEW
#define UT_MAPPLETHANDLEVIEW

#include <QObject>
#include "mapplethandleview.h"
#include "mapplethandleview_p.h"

class MTestAppletHandleView : public MAppletHandleView
{
    M_VIEW(MAppletHandleModel, MAppletHandleStyle)

public:
    MTestAppletHandleView(MAppletHandle *handle);
    void click();
    MAppletHandleModel* modifiableModel();
};

// Test case must inherit QObject
class Ut_MAppletHandleView : public QObject
{
    Q_OBJECT

private:
    //! MAppletHandleView instance under testing.
    MTestAppletHandleView *handleView;
    MAppletHandle *handle;

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
#endif // UT_MAPPLETHANDLEVIEW
