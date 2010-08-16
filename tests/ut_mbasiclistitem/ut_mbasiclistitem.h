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

#ifndef UT_MBASICLISTITEM_H
#define UT_MBASICLISTITEM_H

#include <QtTest/QtTest>
#include <QObject>

// the real unit/mbasiclistitem class declaration
#include <mbasiclistitem.h>

Q_DECLARE_METATYPE(MBasicListItem *);

class Ut_MBasicListItem : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testConstructor();
    void testTitle();
    void testSubtitle();
    void testItemStyle();
    void testImageWidget();

private:
    MBasicListItem *m_subject;
};

#endif
