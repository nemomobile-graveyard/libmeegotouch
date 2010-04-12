#ifndef PANNINGBENCHMARK_H
#define PANNINGBENCHMARK_H

#include "timedemobenchmark.h"

#include <MPannableViewport>
#include <QTime>

/**
  * PanningBenchmark emulates panning a page by scrolling down until the
  * bottom of the page is reached.
  */
class PanningBenchmark : public TimedemoBenchmark
{
    Q_OBJECT
public:
    PanningBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo);

    QString name();
    void start();

private slots:
    void waitBeforePanning();
    void panDown();
    void terminateBenchmark();

private:
    MPannableViewport *pannableViewport;

    bool timingStarted;

    QPointF formerPosition;

    QTime timer;

};

#endif // PANNINGBENCHMARK_H
