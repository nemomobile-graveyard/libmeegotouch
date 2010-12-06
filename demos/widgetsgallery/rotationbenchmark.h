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

#ifndef ROTATIONBENCHMARK_H
#define ROTATIONBENCHMARK_H

#include "timedemobenchmark.h"

#include <MNamespace>

/**
  * Rotate from the current angle to the given one and measures
  * the FPS while doing the rotation.
  */
class RotationBenchmark : public TimedemoBenchmark
{
    Q_OBJECT
public:
    RotationBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo, M::OrientationAngle targetOrientationAngle);

    QString name();
    void start();

private slots:
    void stabilizeFps();
    void rotate();
    void terminateBenchmark();

private:
     M::OrientationAngle targetOrientationAngle;
};

#endif // ROTATIONBENCHMARK_H
