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

#ifndef UT_MCONTENTITEM_H
#define UT_MCONTENTITEM_H

#include <QtTest/QtTest>
#include <QObject>

#include <mcontentitem.h>

Q_DECLARE_METATYPE(MContentItem *);

class Ut_MContentItem : public QObject
{
    Q_OBJECT

private slots:
    void init();
    void cleanup();
    void initTestCase();
    void cleanupTestCase();

    void testSetPixmap_data();
    void testSetPixmap();

    void testSetImage_data();
    void testSetImage();
    void testSetImageID();
    void testSetOptionalPixmap_data();
    void testSetOptionalPixmap();
    void testSetOptionalImage_data();
    void testSetOptionalImage();
    void testSetOptionalImageID();
    void testSetSmallItem();
    void testSetSmallText();
    void testAdditionalItem();
    void testModelSetItemPixmap_data();
    void testModelSetItemPixmap();
    void testModelSetOptionalPixmap_data();
    void testModelSetOptionalPixmap();
    void testModelSetOptionalImage_data();
    void testModelSetOptionalImage();
    void testModelSetItemQImage_data();
    void testModelSetItemQImage();
    void testTitle();
    void testSubtitle();
    void testItemMode();
    void testProgressBar();

private:
    MContentItem *m_subject;
};

#endif
