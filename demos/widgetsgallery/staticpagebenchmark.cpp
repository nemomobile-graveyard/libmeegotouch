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
    MApplication::activeWindow()->setOrientationAngle(targetOrientationAngle);
    MApplication::activeWindow()->setOrientationAngleLocked(true);
    if (!applicationPage->isActiveWindow()) {
        connect(applicationPage, SIGNAL(appeared()), this, SLOT(stabilizeFps()));
        applicationPage->appear();
        verifyAppearanceTimer->start(2000);
    } else {
        QTimer::singleShot(0, this, SLOT(stabilizeFps()));
    }
}

void StaticPageBenchmark::stabilizeFps() {
    verifyAppearanceTimer->stop();
    QTimer::singleShot(1000, this, SLOT(waitPageDuration()));
}

void StaticPageBenchmark::waitPageDuration()
{
    timedemo->startTiming();
    QTimer::singleShot(pageDuration, this, SLOT(terminateBenchmark()));
}

void StaticPageBenchmark::terminateBenchmark()
{
    timedemo->stopTiming();
    emit finished();
}
