#include "timedemobenchmark.h"

TimedemoBenchmark::TimedemoBenchmark(MApplicationPage *applicationPage, Timedemo *timedemo)
    : applicationPage(applicationPage)
    , timedemo(timedemo)
    , _type("common")
{
}

QString TimedemoBenchmark::type() {
    return _type;
}

void TimedemoBenchmark::setType(const QString& type) {
    _type = type;
}
