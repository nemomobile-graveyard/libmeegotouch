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
