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

#ifndef UT_MPANRECOGNIZER_H
#define UT_MPANRECOGNIZER_H

#include <QObject>

class MPanRecognizer;
class MPanGesture;

class Ut_MPanRecognizer : public QObject
{
    Q_OBJECT

private:
    MPanRecognizer*  recognizer;
    MPanGesture*     panGesture;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testCreateGesture();
    void testRecognize();
    void testTapIsNotRecognizedAsPan();
    void testTheMovementInDirectionOtherThanRecognizedIsZeroed();
    void testReset();
};

#endif // UT_MPANRECOGNIZER_H
