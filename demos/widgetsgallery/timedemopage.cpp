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
#include "timedemopage.h"
#include "timedemobenchmark.h"

#include "staticpagebenchmark.h"
#include "panningbenchmark.h"
#include "rotationbenchmark.h"

TimedemoPage::TimedemoPage(QGraphicsItem *parent)
    : MApplicationPage(parent),
    m_benchmarks()
{
}

void TimedemoPage::createBenchmarks(Timedemo *timedemo)
{
    // add a default set of benchmarks
    // inheriting pages may remove or add benchmarks
    QSharedPointer<TimedemoBenchmark> benchmark(new StaticPageBenchmark(this, timedemo, M::Angle0));
    m_benchmarks.append(benchmark);

    benchmark = QSharedPointer<TimedemoBenchmark>(new PanningBenchmark(this, timedemo));
    m_benchmarks.append(benchmark);

    benchmark = QSharedPointer<TimedemoBenchmark>(new RotationBenchmark(this, timedemo, M::Angle90));
    m_benchmarks.append(benchmark);

    benchmark = QSharedPointer<TimedemoBenchmark>(new StaticPageBenchmark(this, timedemo, M::Angle90));
    m_benchmarks.append(benchmark);

    benchmark = QSharedPointer<TimedemoBenchmark>(new PanningBenchmark(this, timedemo, M::Angle90));
    m_benchmarks.append(benchmark);

    benchmark = QSharedPointer<TimedemoBenchmark>(new RotationBenchmark(this, timedemo, M::Angle0));
    m_benchmarks.append(benchmark);
}

QVector<QSharedPointer<TimedemoBenchmark> > TimedemoPage::benchmarks()
{
    return m_benchmarks;
}
