#include "timedemopage.h"
#include "timedemobenchmark.h"

#include "staticpagebenchmark.h"

TimedemoPage::TimedemoPage(QGraphicsItem *parent)
    : DuiApplicationPage(parent)
{
}

void TimedemoPage::createBenchmarks(Timedemo *timedemo)
{
    // add a default set of benchmarks
    // inheriting pages may remove or add benchmarks
    QSharedPointer<TimedemoBenchmark> benchmark(new StaticPageBenchmark(this, timedemo, Dui::Angle0));
    m_benchmarks.append(benchmark);

    benchmark = QSharedPointer<TimedemoBenchmark>(new StaticPageBenchmark(this, timedemo, Dui::Angle90));
    m_benchmarks.append(benchmark);
}

QVector<QSharedPointer<TimedemoBenchmark> > TimedemoPage::benchmarks()
{
    return m_benchmarks;
}
