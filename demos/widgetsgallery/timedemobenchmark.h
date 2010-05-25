#ifndef TIMEDEMOBENCHMARK_H
#define TIMEDEMOBENCHMARK_H

#include <QObject>

class MApplicationPage;
class Timedemo;
class QString;
class QTimer;

/**
  * TimedemoBenchmark is the base class for all benchmarks used in the timedemo.
  * Individual TimedemoBenchmarks can be added to instances of TimedemoPages.
  */
class TimedemoBenchmark : public QObject
{
    Q_OBJECT
public:
    /**
      * \param applicationPage The MApplicationPage we are executing the benchmark with.
      * \param timedemo An instance of the Timedemo class. TimedemoBenchmark must call
      *        Timedemo::startTiming() and TimedemoBenchmark::stopTiming() when the actual
      *        benchmarking phase starts or is stopped.
      */
    TimedemoBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo);

    /**
      * Override this method to return the name of the benchmark.
      */
    virtual QString name() = 0;

    /**
      * This method is called when the Timedemo wants to start this benchmark.
      */
    virtual void start() = 0;

    /**
      * Return the type of the benchmark.
      * \return The type of the benchmark-
      */
    QString type();

    /**
      * Set the type of the benchmark. Evaluation of the timedemo can use this
      * information to handle different types in a different way.
      * \param type The type of the benchmark
      */
    void setType(const QString& type);

signals:
    /**
      * Emit this signal once the benchmark is finished.
      */
    void finished();

protected slots:
    void terminateIfNotAppeared();

protected:
    /**
       * Start this timer after calling MApplicationPage::appear() to verify the application page
       * has really appeared/is really active.
       * Timer will terminate the application after the timeout if page is not active.
       */
    QTimer *verifyAppearanceTimer;
    MApplicationPage *applicationPage;
    Timedemo *timedemo;
    QString _type;
};

#endif // TIMEDEMOBENCHMARK_H
