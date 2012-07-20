/***************************************************************************
**
** Copyright (C) 2012 Jolla Ltd.
** All rights reserved.
** Contact: Jolla Ltd. (info@jollamobile.com)
**
** This file is part of libmeegotouch.
**
** If you have questions regarding the use of this file, please contact
** Jolla at info@jollamobile.com.
**
** This library is free software; you can redistribute it and/or
** modify it under the terms of the GNU Lesser General Public
** License version 2.1 as published by the Free Software Foundation
** and appearing in the file LICENSE.LGPL included in the packaging
** of this file.
**
****************************************************************************/

#include <QDateTime>
#include "logger.h"

Logger::Logger(const QString &logPath, const QString &logLevel) :
    m_logfile(NULL),
    m_outputLevel(QtDebugMsg),
    m_gconfLogPath("/meegotouch/theme/logpath"),
    m_gconfLogLevel("/meegotouch/theme/loglevel")
{
    if (!logPath.isEmpty()) {
        setLogPath(logPath);
    } else if (!m_gconfLogPath.value().isNull()) {
        // use GConf for default log path
        setLogPath(m_gconfLogPath.value().toString());
    }

    if (!logLevel.isEmpty()) {
        setLogLevel(logLevel);
    } else if (!m_gconfLogLevel.value().isNull()) {
        // use GConf for default log path
        setLogLevel(m_gconfLogLevel.value().toString());
    }

    // reopen log file if re-configured
    connect(&m_gconfLogPath,
            SIGNAL(valueChanged()),
            this,
            SLOT(logPathChanged()));
    connect(&m_gconfLogLevel,
            SIGNAL(valueChanged()),
            this,
            SLOT(logLevelChanged()));
}

Logger::~Logger() {}

void Logger::log(const QtMsgType type, const QString &msg)
{
    if (type < m_outputLevel)
        return;
    if (!m_logstream.device())
        return;

    QString severity;

    switch (type) {
        case QtDebugMsg:
            severity = "debug ";
            break;
        case QtWarningMsg:
            severity = "warning ";
            break;
        case QtCriticalMsg:
            severity = "critical ";
            break;
        case QtFatalMsg:
            severity = "fatal ";
    }
    m_logstream << QDateTime::currentDateTime().toString("[dd.MM.yyyy hh:mm:ss.zzz] ") << severity << msg << "\n";
    m_logstream.flush();
}

void Logger::setLogPath(const QString &logPath)
{
    if (m_logfile) {
        m_logfile->close();
        delete m_logfile;
        m_logfile = NULL;
    }
    if (!logPath.isEmpty()) {
        m_logfile = new QFile(logPath, this);
        if (!m_logfile->open(QIODevice::Append | QIODevice::Text)) {
            fprintf(stderr, "%s\n", "Can't open log file");
            delete m_logfile;
            m_logfile = NULL;
            return;
        }
        m_logstream.setDevice(m_logfile);
    }
}

void Logger::setLogLevel(const QString &logLevel)
{
    if (logLevel == "debug") {
        m_outputLevel = QtDebugMsg;
    } else if (logLevel == "warning") {
        m_outputLevel = QtWarningMsg;
    } else if (logLevel == "critical") {
        m_outputLevel = QtCriticalMsg;
    } else {
#ifdef __arm__
        m_outputLevel = QtCriticalMsg;
#else
        m_outputLevel = QtWarningMsg;
#endif
    }
}

void Logger::logPathChanged()
{
    if (m_gconfLogPath.value().isNull()) {
        setLogPath(QString());
    } else {
        setLogPath(m_gconfLogPath.value().toString());
    }
}

void Logger::logLevelChanged()
{
    if (m_gconfLogLevel.value().isNull()) {
        setLogLevel(QString());
    } else {
        setLogLevel(m_gconfLogLevel.value().toString());
    }
}
