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

#ifndef UT_DUIPANNABLEVIEWPORT_H
#define UT_DUIPANNABLEVIEWPORT_H

#include <duipannableviewport.h>

#include <QtTest/QtTest>

class Ut_DuiPannableViewport : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void cleanupTestCase();

    void init();
    void cleanup();

    void constructor();
    void setWidget();
    void setGeometry_data();
    void setGeometry();
    void event_data();
    void event();
    void updatePosition();
    void updateSamePosition();
    void sizePosChangedAfterPopulatingPannedWidget();

    //Changing position indicator in the viewport
    void settingNewPositionIndicator();
    void settingNULLPositionIndicatorShouldNotBeAccepted();

private:
    DuiPannableViewport *subject;
};

#endif
