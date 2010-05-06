#ifndef TIMEDEMOPAGE_H
#define TIMEDEMOPAGE_H

#include <MApplicationPage>

#include <QList>
#include <QSharedPointer>

#include "timedemobenchmark.h"

class Timedemo;

/**
  * Base class for all pages which are offering a demo facility.
  * Arbitrary benchmarks can be added to this page and will then
  * be executed by the Timedemo.
  */
class TimedemoPage : public MApplicationPage
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
      * For visualizing the timedemo results this method should return an
      * unique identifier for the page. Make sure to never change the
      * returned title once your page has been in a release as this would
      * invalidate older results.
      * \return the title to show when visualizing timedemo results
      */
    virtual QString timedemoTitle() = 0;

    /**
      * Returns the benchmarks for this page.
      */
    QVector<QSharedPointer<TimedemoBenchmark> > benchmarks();

protected:
    QVector<QSharedPointer<TimedemoBenchmark> > m_benchmarks;
};

#endif // TIMEDEMOPAGE_H
