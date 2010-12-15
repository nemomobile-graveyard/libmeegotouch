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

#ifndef UT_MCOMMONPIXMAPS_H
#define UT_MCOMMONPIXMAPS_H

#include <QObject>
#include <QtTest/QtTest>

class MApplication;
class MThemeDaemon;
class MCommonPixmaps;
class PixmapCacheEntry;

class Ut_MCommonPixmaps : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void testConstructor();
    void testLoad();
    void testIncreaseRequestCount();
    void testReload();
    void testSaveAndClear();

private:
    MApplication *m_app;
    MCommonPixmaps *m_commonPixmaps;
    MThemeDaemon *m_themeDaemon;
    QList<PixmapCacheEntry*> pixmapsToDelete;
};
#endif
