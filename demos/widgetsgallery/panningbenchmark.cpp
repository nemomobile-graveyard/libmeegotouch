#include "panningbenchmark.h"
#include "timedemo.h"

#include <DuiApplicationPage>
#include <DuiPannableViewport>
#include <DuiPhysics2DPanning>

#include <QTimer>
#include <qdebug.h>

namespace {
    const int updateInterval = 20; // ms
    const int yOffset = 20;
}

PanningBenchmark::PanningBenchmark(DuiApplicationPage *applicationPage, Timedemo *timedemo)
    : TimedemoBenchmark(applicationPage, timedemo)
    , timingStarted(false)
{
    pannableViewport = 0;
    QList<QGraphicsItem *> childItems = applicationPage->childItems();
    foreach(QGraphicsItem * childItem, childItems) {
        if (DuiPannableViewport * viewport = dynamic_cast<DuiPannableViewport*>(childItem)) {
            pannableViewport = viewport;
            break;
        }
    }

    if (!pannableViewport) {
        qFatal("Did not find matching viewport of DuiApplicationWindow");
    }
}

QString PanningBenchmark::name() {
    return "PanningBenchmark";
}

void PanningBenchmark::start()
{
    if (!applicationPage->isActiveWindow()) {
        connect(applicationPage, SIGNAL(windowShown()), this, SLOT(waitBeforePanning()));
        applicationPage->appear();
    } else {
        QTimer::singleShot(0, this, SLOT(panDown()));
    }
}

//HACK! if we do not wait pannableViewport->physics()->range() is (0, 0, 0, 0) because
// the widgets are not completely set up yet
void PanningBenchmark::waitBeforePanning()
{
    QTimer::singleShot(500, this, SLOT(panDown()));
}

void PanningBenchmark::panDown()
{
    if (!timingStarted) {
        timedemo->startTiming();
        timingStarted = true;
        formerPosition = pannableViewport->physics()->position();
    }

    QPointF currentPosition = pannableViewport->physics()->position();
    QRectF range = pannableViewport->physics()->range();

    if (currentPosition.y() < range.height()) {
        pannableViewport->physics()->setPosition(currentPosition + QPointF(0., yOffset));
        QTimer::singleShot(20, this, SLOT(panDown()));
    } else {
        terminateBenchmark();
    }

}

void PanningBenchmark::terminateBenchmark()
{
    timedemo->stopTiming();
    pannableViewport->physics()->setPosition(formerPosition);

    qDebug() << "end" << pannableViewport->widget()->pos() << pannableViewport->physics()->range();

    emit finished();
}
