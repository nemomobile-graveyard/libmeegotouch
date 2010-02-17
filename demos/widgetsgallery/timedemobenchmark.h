#ifndef TIMEDEMOBENCHMARK_H
#define TIMEDEMOBENCHMARK_H

#include <QObject>

class DuiApplicationPage;
class Timedemo;
class QString;

/**
  * TimedemoBenchmark is the base class for all benchmarks used in the timedemo.
  * Individual TimedemoBenchmarks can be added to instances of TimedemoPages.
  */
class TimedemoBenchmark : public QObject
{
    Q_OBJECT
public:
    /**
      * \param applicationPage The DuiApplicationPage we are executing the benchmark with.
      * \param timedemo An instance of the Timedemo class. TimedemoBenchmark must call
      *        Timedemo::startTiming() and TimedemoBenchmark::stopTiming() when the actual
      *        benchmarking phase starts or is stopped.
      */
    TimedemoBenchmark(DuiApplicationPage *applicationPage, Timedemo *timedemo);

    /**
      * Override this method to return the name of the benchmark.
      */
    virtual QString name() = 0;

    /**
      * This method is called when the Timedemo wants to start this benchmark.
      */
    virtual void start() = 0;

signals:
    /**
      * Emit this signal once the benchmark is finished.
      */
    void finished();

protected:
    DuiApplicationPage *applicationPage;
    Timedemo *timedemo;
};

#endif // TIMEDEMOBENCHMARK_H
