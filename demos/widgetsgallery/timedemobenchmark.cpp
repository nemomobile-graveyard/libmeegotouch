#include "timedemobenchmark.h"
#include "timedemo.h"

#include <QTimer>
#include <MApplicationPage>

TimedemoBenchmark::TimedemoBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo)
    : QObject(),
    verifyAppearanceTimer(0),
    applicationPage(applicationPage),
    timedemo(timedemo),
    _type("common")
{
    verifyAppearanceTimer = new QTimer(this);
    connect(verifyAppearanceTimer, SIGNAL(timeout()), this, SLOT(terminateIfNotAppeared()));
}

QString TimedemoBenchmark::type() {
    return _type;
}

void TimedemoBenchmark::setType(const QString& type) {
    _type = type;
}

void TimedemoBenchmark::terminateIfNotAppeared() {
    verifyAppearanceTimer->stop();
    if (applicationPage->isOnDisplay()) {
        return;
    } else {
        timedemo->displayBenchmarkResults();
        qCritical("Could not activate application page. Either the application page is not the toplevel window or the window manager is not running.\nAborting.");
        exit(EXIT_FAILURE);
    }
}
