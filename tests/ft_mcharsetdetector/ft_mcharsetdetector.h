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

#ifndef FT_MCHARSETDETECTOR_H
#define FT_MCHARSETDETECTOR_H

#include <QtTest/QtTest>
#include <QObject>
#include <Qt>
#include <QTextCodec>

#include <MApplication>

#include <MCharsetDetector>
#include <MCharsetMatch>

class Ft_MCharsetDetector : public QObject
{
    Q_OBJECT

private:
    MApplication *qap;

private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testMCharsetMatch_data();
    void testMCharsetMatch();

    void testConstructors_data();
    void testConstructors();

    void testInputFilterEnabledDefaultValue();

    void testDetectableCharsets_data();
    void testDetectableCharsets();

    void testDetection_data();
    void testDetection();
};

#endif
