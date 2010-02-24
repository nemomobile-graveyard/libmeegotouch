#include "staticpagebenchmark.h"
#include "timedemo.h"

#include <DuiApplication>
#include <DuiApplicationPage>

#include <QTimer>

namespace
{
    const int pageDuration = 4000;
}

StaticPageBenchmark::StaticPageBenchmark(DuiApplicationPage *applicationPage, Timedemo *timedemo, Dui::OrientationAngle targetOrientationAngle)
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
        applicationPage->setEscapeButtonMode(DuiEscapeButtonPanelModel::BackMode);
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
    formerOrientationAngle = DuiApplication::activeWindow()->orientationAngle();
    DuiApplication::activeWindow()->setOrientationAngleLocked(false);
    DuiApplication::activeWindow()->setOrientationAngle(targetOrientationAngle, Dui::ImmediateOrientationChange);
    DuiApplication::activeWindow()->setOrientationAngleLocked(true);

    timedemo->startTiming();
    QTimer::singleShot(pageDuration, this, SLOT(terminateBenchmark()));
}

void StaticPageBenchmark::terminateBenchmark()
{
    timedemo->stopTiming();

    DuiApplication::activeWindow()->setOrientationAngleLocked(false);
    DuiApplication::activeWindow()->setOrientationAngle(formerOrientationAngle, Dui::ImmediateOrientationChange);
    DuiApplication::activeWindow()->setOrientationAngleLocked(true);
    emit finished();
}
