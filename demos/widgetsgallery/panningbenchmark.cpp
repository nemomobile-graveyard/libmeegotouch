#include "panningbenchmark.h"
#include "timedemo.h"

#include <MApplication>
#include <MApplicationPage>
#include <MPannableViewport>
#include <MPhysics2DPanning>

#include <QTimer>
#include <qdebug.h>


namespace {
    const int updateInterval = 20; // ms
    const qreal ySpeed = .5; // pixel/ms
}

PanningBenchmark::PanningBenchmark(
  MApplicationPage * applicationPage, Timedemo * timedemo, M::OrientationAngle targetOrientationAngle) :
      TimedemoBenchmark(applicationPage, timedemo),
      timingStarted(false),
      targetOrientationAngle(targetOrientationAngle)

{
    pannableViewport = 0;
    QList<QGraphicsItem *> childItems = applicationPage->childItems();
    foreach(QGraphicsItem * childItem, childItems) {
        if (MPannableViewport * viewport = dynamic_cast<MPannableViewport*>(childItem)) {
            pannableViewport = viewport;
            break;
        }
    }

    if (!pannableViewport) {
        qFatal("Did not find matching viewport of MApplicationWindow");
    }
}

QString PanningBenchmark::name() {
    return QString("PanningBenchmark (%1)").arg(QString::number(targetOrientationAngle));
}

void PanningBenchmark::start()
{
    MApplication::activeWindow()->setOrientationAngle(targetOrientationAngle);
    MApplication::activeWindow()->setOrientationAngleLocked(true);
    if (!applicationPage->isOnDisplay()) {
        connect(applicationPage, SIGNAL(appeared()), this, SLOT(waitBeforePanning()));
        applicationPage->appear(MApplication::activeWindow());
        verifyAppearanceTimer->start(5000);
    } else {
        waitBeforePanning();
    }
}

//HACK! if we do not wait pannableViewport->physics()->range() is (0, 0, 0, 0) because
// the widgets are not completely set up yet
void PanningBenchmark::waitBeforePanning()
{
    verifyAppearanceTimer->stop();
    QTimer::singleShot(2500, this, SLOT(panDown()));
}

void PanningBenchmark::panDown()
{
    if (!timingStarted) {
        timedemo->startTiming();
        timingStarted = true;
        formerPosition = pannableViewport->physics()->position();
        timer.start();
    }

    QPointF currentPosition = pannableViewport->physics()->position();
    QRectF range = pannableViewport->physics()->range();

    if (currentPosition.y() < range.height()) {
        pannableViewport->physics()->setPosition(currentPosition + QPointF(0., ySpeed * timer.elapsed()));
        timer.start();
        QTimer::singleShot(updateInterval, this, SLOT(panDown()));
    } else {
        terminateBenchmark();
    }
}

void PanningBenchmark::terminateBenchmark()
{
    timedemo->stopTiming();
    pannableViewport->physics()->setPosition(formerPosition);
    emit finished();
}
