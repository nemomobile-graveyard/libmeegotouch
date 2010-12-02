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

#ifndef FT_MSCALABLEIMAGE_H
#define FT_MSCALABLEIMAGE_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>

class Ft_MScalableImage : public QObject
{
    Q_OBJECT
public:
    Ft_MScalableImage();

private slots:
    void init();
    void cleanup();

    void initTestCase();
    void cleanupTestCase();

    void test_draw_scalable9();
    void test_draw_scalable1();

    void testEnableOptimizedRendering();

private:
    void verifyResult(const QImage& image, const QSize& paintSize);
    class MScalableImage *m_subject;
};

#endif

