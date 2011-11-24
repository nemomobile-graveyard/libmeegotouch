/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef UT_MTAPSTATEMACHINE_H
#define UT_MTAPSTATEMACHINE_H

#include <QGraphicsWidget>

class MTapStateMachine;

class Ut_MTapStateMachine : public QObject
{
    Q_OBJECT

private:
    MTapStateMachine*  machine;
    QGraphicsWidget* fakeObject;
private slots:
    void initTestCase();
    void cleanupTestCase();
    void init();
    void cleanup();

    void testLongPressAndRelease();
    void testShortTapAndRelease();
    void testLongPressAndHide();

};

#endif // UT_MTAPSTATEMACHINE_H
