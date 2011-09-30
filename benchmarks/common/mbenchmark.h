/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

// QBENCHARK_ONCE causes the code to be executed twice
// only the runtime of the second run is actually measured.
// measuring the time needed to initialize static data is
// therefor not possible. to work around this we just execute
// the real code in the second itereation.
// this macro is also handy when you allocate memory but do
// not free it in the test slot. with QBENCHMARK_ONCE you have
// a memory leak in this case.
// ATTENTION: make sure the code you want to measure works with
// this hack
// See <http://bugreports.qt.nokia.com/browse/QTBUG-12689>
#define MBENCHMARK_ONCE(CODE) \
    QBENCHMARK_ONCE { \
        static bool firstRun = true; \
        if (firstRun) { \
            firstRun = false; \
        } else { \
            CODE \
        } \
    }
