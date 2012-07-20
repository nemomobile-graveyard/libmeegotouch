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

#ifndef LOGGER_H
#define LOGGER_H

#include <QFile>
#include <QTextStream>
#include <MGConfItem>

//! \internal
class Logger : public QObject
{
    Q_OBJECT
public:
    Logger(const QString &logPath = QString(), const QString &logLevel = QString());
    virtual ~Logger();

    void log(const QtMsgType type, const QString &msg);
    void setLogPath(const QString &logPath);
    void setLogLevel(const QString &logLevel);

private slots:
    void logPathChanged();
    void logLevelChanged();

private:
    QFile* m_logfile;
    QTextStream m_logstream;
    QtMsgType m_outputLevel;
    MGConfItem m_gconfLogPath;
    MGConfItem m_gconfLogLevel;
};

#endif
