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

#ifndef PT_MCOMPONENTCACHE_H
#define PT_MCOMPONENTCACHE_H

#include <QtTest/QtTest>
#include <QObject>

/**
 * Benchmark for MComponentCache, which uses MComponentData::reinit()
 * to reinitialize the component data. MComponentData::reinit() cannot
 * be benchmarked within pt_mcomponentdata, as there is an internal
 * dependency to MComponentCache.
 */
class Pt_MComponentCache : public QObject
{
    Q_OBJECT

private slots:
    void reinit();
};

#endif
