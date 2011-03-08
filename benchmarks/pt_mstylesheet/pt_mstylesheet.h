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

#ifndef PT_MSTYLESHEET_H
#define PT_MSTYLESHEET_H

#include <QObject>

class MLogicalValues;

/**
  * This test benchmarks the MStyleSheet loading performance.
  */
class Pt_MStyleSheet : public QObject
{
    Q_OBJECT

private slots:
    void constructor();

    /**
     * Benchmark the loading of theme styles that are typically loaded
     * when starting an application.
     */
    void loadThemeStyles();
    void loadThemeStyles_data();

    /**
     * Some core styles are loaded multiple times during startup. Benchmark
     * whether multiple loading results in a linear increasing of the
     * startup time.
     */
    void loadCoreStyles();
    void loadCoreStyles_data();

    void testBinarySpeed();

private:
    void initLogicalValues(MLogicalValues &values);
};

#endif
