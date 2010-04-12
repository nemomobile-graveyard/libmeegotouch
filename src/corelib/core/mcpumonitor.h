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

#ifndef MCPUMONITOR_H
#define MCPUMONITOR_H


#include <QObject>


class MCpuMonitorPrivate;

//! \internal

/*!
 * Utility class to monitor CPU usage on system
 *
 * Gets frame every timeout and has function to get usage for last frame. The
 * timer must be started in order to get frames.
 */
class MCpuMonitor : public QObject
{
    Q_OBJECT

public:
    MCpuMonitor();
    ~MCpuMonitor();

    /*!
     * Gets CPU usage for last frame.
     *
     * \return CPU usage for last frame in %. If there is no data avaialble
     * yet, -1 is returned.
     */
    int usage();

public Q_SLOTS:
    /*!
     * Starts the timer. Each time timer times out, new CPU frame becomes
     * available and newCpuFrameAvailable signal is emitted.
     *
     * \param msec Timeout in milliseconds.
     */
    void start(unsigned msec);

    /*!
     * Stops the timer. CPU usage frames will be no longer read.
     */
    void stop();

Q_SIGNALS:
    /*!
     * Emitted when there is new CPU frame available.
     */
    void newCpuFrameAvailable();

private Q_SLOTS:
    void timedOut();

private:
    MCpuMonitorPrivate *const d_ptr;
    Q_DECLARE_PRIVATE(MCpuMonitor)
};

//! \internal_end

#endif
