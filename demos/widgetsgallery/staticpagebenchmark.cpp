#include "staticpagebenchmark.h"
#include "timedemo.h"

#include <MApplication>
#include <MApplicationPage>

#include <QTimer>

namespace
{
    const int pageDuration = 3000;
}

StaticPageBenchmark::StaticPageBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo, M::OrientationAngle targetOrientationAngle)
    : TimedemoBenchmark(applicationPage, timedemo)
    , targetOrientationAngle(targetOrientationAngle)
{
}

QString StaticPageBenchmark::name()
{
    return QString("StaticPageBenchmark (%1)").arg(QString::number(targetOrientationAngle));
}

void StaticPageBenchmark::start()
{
    if (!applicationPage->isActiveWindow()) {
        connect(applicationPage, SIGNAL(appeared()), this, SLOT(stabilizeFps()));
        applicationPage->appear();
    } else {
        QTimer::singleShot(0, this, SLOT(stabilizeFps()));
    }
}

void StaticPageBenchmark::stabilizeFps() {
    QTimer::singleShot(1000, this, SLOT(waitPageDuration()));
}

void StaticPageBenchmark::waitPageDuration()
{
    formerOrientationAngle = MApplication::activeWindow()->orientationAngle();
    MApplication::activeWindow()->setOrientationAngleLocked(false);
    MApplication::activeWindow()->setOrientationAngle(targetOrientationAngle);
    MApplication::activeWindow()->setOrientationAngleLocked(true);

    timedemo->startTiming();
    QTimer::singleShot(pageDuration, this, SLOT(terminateBenchmark()));
}

void StaticPageBenchmark::terminateBenchmark()
{
    timedemo->stopTiming();

    MApplication::activeWindow()->setOrientationAngleLocked(false);
    MApplication::activeWindow()->setOrientationAngle(formerOrientationAngle);
    MApplication::activeWindow()->setOrientationAngleLocked(true);
    emit finished();
}
