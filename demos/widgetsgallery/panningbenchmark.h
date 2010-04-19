#ifndef PANNINGBENCHMARK_H
#define PANNINGBENCHMARK_H

#include "timedemobenchmark.h"

#include <MPannableViewport>
#include <MNamespace>

#include <QTime>

/**
  * PanningBenchmark emulates panning a page by scrolling down until the
  * bottom of the page is reached.
  */
class PanningBenchmark : public TimedemoBenchmark
{
    Q_OBJECT
public:
    PanningBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo, M::OrientationAngle targetOrientationAngle = M::Angle0);

    QString name();
    void start();

private slots:
    void waitBeforePanning();
    void panDown();
    void terminateBenchmark();

    void setAngle();
    void resetAngle();

private:
    MPannableViewport *pannableViewport;

    bool timingStarted;

    QPointF formerPosition;

    QTime timer;

private:
     M::OrientationAngle targetOrientationAngle;
     M::OrientationAngle formerOrientationAngle;

};

#endif // PANNINGBENCHMARK_H
