#ifndef STATICPAGEBENCHMARK_H
#define STATICPAGEBENCHMARK_H

#include "timedemobenchmark.h"

#include <DuiNamespace>

/**
  * Static page benchmark simply shows a given page and then waits five seconds.
  * The page is shown in angle given to the constructor.
  */
class StaticPageBenchmark : public TimedemoBenchmark
{
    Q_OBJECT
public:
    StaticPageBenchmark(DuiApplicationPage *applicationPage, Timedemo *timedemo, Dui::OrientationAngle targetOrientationAngle);

    QString name();
    void start();

private slots:
    void stabilizeFps();
    void waitPageDuration();
    void terminateBenchmark();

private:
     Dui::OrientationAngle targetOrientationAngle;
     Dui::OrientationAngle formerOrientationAngle;
};

#endif // STATICPAGEBENCHMARK_H
