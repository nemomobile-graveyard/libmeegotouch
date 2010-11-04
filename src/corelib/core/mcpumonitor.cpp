/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
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

#include <QTimer>
#include <QFile>
#include <QString>
#include <QStringList>

#include "mcpumonitor.h"


class MCpuMonitorPrivate
{
public:
    QTimer timer;

    bool usable;
    int lastFrameUsage;

#ifdef __linux__
    unsigned long jiffies_total;
    unsigned long jiffies_idle;

    void getCpuUsageInformation(unsigned long &total, unsigned long &idle);
#endif
};


MCpuMonitor::MCpuMonitor() : d_ptr(new MCpuMonitorPrivate)
{
    Q_D(MCpuMonitor);

    d->usable = false;

#ifdef __linux__
    d->getCpuUsageInformation(d->jiffies_total, d->jiffies_idle);
#endif

    connect(&d->timer, SIGNAL(timeout()), SLOT(timedOut()));
}

MCpuMonitor::~MCpuMonitor()
{
    Q_D(MCpuMonitor);

    d->timer.stop();

    delete d_ptr;
}


int MCpuMonitor::usage()
{
    Q_D(MCpuMonitor);

    if (!d->usable)
        return -1;

    return d->lastFrameUsage;
}


void MCpuMonitor::start(unsigned msec)
{
    Q_D(MCpuMonitor);
#ifdef __linux__
    d->getCpuUsageInformation(d->jiffies_total, d->jiffies_idle);
#endif
    d->timer.start(msec);
}

void MCpuMonitor::stop()
{
    Q_D(MCpuMonitor);

    d->timer.stop();
}


void MCpuMonitor::timedOut()
{
#ifdef __linux__
    Q_D(MCpuMonitor);

    unsigned long n_tot = 0;
    unsigned long n_idle = 0;

    d->getCpuUsageInformation(n_tot, n_idle);

    if (n_tot - d->jiffies_total) {
        d->lastFrameUsage = 100 - 100 * (n_idle - d->jiffies_idle) / (n_tot - d->jiffies_total);
        d->usable = true;

        d->jiffies_idle = n_idle;
        d->jiffies_total = n_tot;

        emit newCpuFrameAvailable();
    }
#endif
}


#ifdef __linux__
void MCpuMonitorPrivate::getCpuUsageInformation(unsigned long &total, unsigned long &idle)
{
    QFile statFile("/proc/stat");

    if (statFile.exists() && statFile.open(QIODevice::ReadOnly)) {
        char buf[256];
        QStringList l;
        unsigned long sum = 0;
        unsigned long tmp;

        statFile.readLine(buf, 256);
        l = QString::fromAscii(buf).split(' ');

        int idx = 0;
        QStringList::const_iterator end = l.constEnd();
        for (QStringList::const_iterator i = l.constBegin(); i != end; ++i) {
            if (!i->isEmpty()) {
                if (idx) {
                    bool success = true;

                    tmp = i->toULong(&success);
                    if (success) {
                        sum += tmp;

                        if (idx == 4)
                            idle = tmp;
                    }
                }

                idx++;
            }
        }

        total = sum;
    }
}
#endif

