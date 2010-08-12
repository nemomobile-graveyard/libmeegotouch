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

#ifndef UT_MDETAILEDLISTITEM_H
#define UT_MDETAILEDLISTITEM_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/mdetailedlistitem class declaration
#include <mdetailedlistitem.h>

Q_DECLARE_METATYPE(MDetailedListItem *);

class Ut_mdetailedlistitem : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testConstructor();
    void testSetItemStyle();
    void testSetIconStyle();
    void testSetImageWidget();
    void testSetTitle();
    void testSetSubtitle();
    void testSetSideBottomTitle();
    void testSideBottomImageWidget();

private:
    MDetailedListItem *m_subject;
};

#endif
