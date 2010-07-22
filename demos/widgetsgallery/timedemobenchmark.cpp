#include "timedemobenchmark.h"

#include <QTimer>
#include <MApplicationPage>

TimedemoBenchmark::TimedemoBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo)
    : applicationPage(applicationPage)
    , timedemo(timedemo)
    , _type("common")
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
        qCritical("Could not activate application page. Is a window manager running?");
        exit(EXIT_FAILURE);
    }
}
