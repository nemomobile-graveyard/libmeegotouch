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

#include "movershotbeziereasingcurve.h"

namespace {
    qreal curve_points[] = {0, 0.0009, 0.0037, 0.0085, 0.0156, 0.0252, 0.0375, 0.0530, 0.0719, 0.0949,
                            0.1226, 0.1557, 0.1954, 0.2432, 0.3014, 0.3736, 0.4658, 0.5866, 0.6698,
                            0.7263, 0.7700, 0.8057, 0.8358, 0.8618, 0.8845, 0.9046, 0.9225, 0.9384,
                            0.9528, 0.9657, 0.9773, 0.9879, 0.9974, 1.0060, 1.0138, 1.0208, 1.0271,
                            1.0327, 1.0377, 1.0422, 1.0461, 1.0496, 1.0525, 1.0551, 1.0572, 1.0589,
                            1.0603, 1.0613, 1.0619, 1.0623, 1.0623, 1.0621, 1.0615, 1.0607, 1.0597,
                            1.0584, 1.0568, 1.0550, 1.0530, 1.0508, 1.0484, 1.0458, 1.0430, 1.0399,
                            1.0368, 1.0340, 1.0313, 1.0289, 1.0266, 1.0244, 1.0224, 1.0206, 1.0188,
                            1.0172, 1.0157, 1.0142, 1.0129, 1.0117, 1.0105, 1.0094, 1.0084, 1.0075,
                            1.0066, 1.0058, 1.0051, 1.0044, 1.0038, 1.0032, 1.0027, 1.0023, 1.0019,
                            1.0015, 1.0012, 1.0009, 1.0006, 1.0004, 1.0003, 1.0002, 1.0001, 1.0000};

    qreal overshotBezierEasingFunction(qreal progress) {
        return curve_points[(int)(progress * (sizeof curve_points / sizeof curve_points[0] - 1))];
    }
}

MOvershotBezierEasingCurve::MOvershotBezierEasingCurve() : QEasingCurve()
{
    setCustomType(overshotBezierEasingFunction);
}
