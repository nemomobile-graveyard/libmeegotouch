/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_MPANRECOGNIZERTOUCH_H
#define UT_MPANRECOGNIZERTOUCH_H

#include <QObject>

class MPanRecognizerTouch;
class MPanGestureTouch;

class Ut_MPanRecognizerTouch : public QObject
{
    Q_OBJECT

public:
    Ut_MPanRecognizerTouch();

private:
    MPanRecognizerTouch *recognizer;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testSimplePanGestureRecognition();
    void testTapIsNotRecognizedAsPan();
    void testTheMovementInDirectionOtherThanRecognizedIsZeroed();

    /*
      Use case:
      1 - user puts finger on touchscreen and starts panning
      2 - user puts a second finger on touchscreen (or touches the screen
          with his palm)
      3 - user keeps panning with his finger from step 1
      4 - user raises finger from step 1.
      5 - user moves finger/contact point from step 2
      6 - user puts another finger on touchscreen and starts panning

      Outcome: panning shouldn't be affected at all by the touch point from step 2.
      That touch point should be completely ignored.
    */
    void testSecondTouchPointHasNoEffectOnSequentialPannings();

};

#endif // UT_MPANRECOGNIZERTOUCH_H
