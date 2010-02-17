#ifndef TIMEDEMOPAGE_H
#define TIMEDEMOPAGE_H

#include <DuiApplicationPage>

#include <QList>
#include <QSharedPointer>

#include "timedemobenchmark.h"

class Timedemo;

/**
  * Base class for all pages which are offering a demo facility.
  * Arbitrary benchmarks can be added to this page and will then
  * be executed by the Timedemo.
  */
class TimedemoPage : public DuiApplicationPage
{
public:
    TimedemoPage(QGraphicsItem *parent = 0);

    /**
      * Timedemo calls this class once before starting any benchmarks
      * for this page. Create the actual benchmarks in this method.
      * If you override this method and want the default benchmarks to be created
      * make sure to explicitly call this implementation.
      */
    virtual void createBenchmarks(Timedemo *timedemo);

    /**
      * Returns the benchmarks for this page.
      */
    QVector<QSharedPointer<TimedemoBenchmark> > benchmarks();

private:
    QVector<QSharedPointer<TimedemoBenchmark> > m_benchmarks;

};

#endif // TIMEDEMOPAGE_H
