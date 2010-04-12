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

#ifndef UT_MSLIDER_H
#define UT_MSLIDER_H

#include <QObject>
#include <QtTest/QtTest>
#include <QString>

class Ut_MSlider : public QObject
{
    Q_OBJECT

private slots:
    void sliderSetMinIndicatorVisibility();
    void sliderSetMaxIndicatorVisibility();
    void sliderSetHandleIndicatorVisibility();
    void sliderSetMinIndicatorText();
    void sliderSetMaxIndicatorText();
    void sliderSetHandleIndicatorText();
    void sliderSetMinImageID();
    void sliderSetMaxImageID();
    void sliderSetHandleImageID();
    void sliderSetMinimum();
    void sliderSetMaximum();
    void sliderSetRange();
    void sliderSetValue();
    void sliderSetSteps();
    void sliderOrientation();
    void sliderState();
};

#endif
