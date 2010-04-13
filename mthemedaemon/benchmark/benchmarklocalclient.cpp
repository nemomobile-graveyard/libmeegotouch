#include <QApplication>

#include "mlocalthemedaemon.h"

#include "benchmarklocalclient.h"
#include "common.h"

BenchmarkLocalClient::BenchmarkLocalClient()
{
}

void BenchmarkLocalClient::run() {
    MLocalThemeDaemon themeDaemon(qApp->applicationName());

    QList<QPair<QString,QSize> > pixmapsToRequest = Common::getPixmapsToRequest();

    QMap<QString, RequestInfo> answeredRequests;
    QListIterator<QPair<QString,QSize> > iter(pixmapsToRequest);
    while (iter.hasNext()) {
        QPair<QString,QSize> request = iter.next();
    
        RequestInfo info;
        info.request = QTime::currentTime();
        themeDaemon.pixmapHandleSync(request.first, request.second);
        info.answer = QTime::currentTime();
        themeDaemon.releasePixmap(request.first, request.second);
        answeredRequests[request.first] = info;
    }

    Common::printResults("local theme daemon", answeredRequests, QMap<QString, RequestInfo>());

    emit finished();
}
