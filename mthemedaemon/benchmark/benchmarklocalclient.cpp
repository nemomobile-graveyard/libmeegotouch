/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Nokia at directui@nokia.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/
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
