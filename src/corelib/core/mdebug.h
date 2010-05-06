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

#ifndef MDEBUG_H
#define MDEBUG_H

#include <QDebug>

/*!
 * \brief Redirects all debugging output from qDebug, qWarning, qCritical and qFatal
 * to specified file.
 * \param filename const char* Specifies file name
 * \return true if file was opened successfully
 */
bool mRedirectOutput(const QString &filename);

#ifdef QT_NO_DEBUG_OUTPUT
#define mDebug(x) qDebug()
#else

/**
 * Returns an object to send debug messages to the message handler.
 * Use as a replacement for qDebug. Pass the name of the module
 * as QString to categorize the output.
 *
 * Example:
 *\code
 * #include <MDebug>
 *
 * mDebug("CSS Parser") << "Reading one line";
 *\endcode
 *
 * The user can now decide which debug messages are visible by command line arguments to MApplication.
 */
inline QDebug mDebugStream(const QString &module)
{
    // Use qPrintable around QString to avoid "" around it
    return qDebug() << qPrintable(QString("%1:").arg(module));
}

#define mDebug(x) mDebugStream(x)
#endif

#ifdef QT_NO_WARNING_OUTPUT
#define mWarning(x) qDebug()
#else

/**
 * Returns an object to send warning messages to the message handler.
 * Use as a replacement for qWarning. Pass the name of the module
 * as QString to categorize the output.
 *
 * \sa mDebug(const QString &)
 */
inline QDebug mWarningStream(const QString &module)
{
    // Use qPrintable around QString to avoid "" around it
    return qWarning() << qPrintable(QString("%1:").arg(module));
}

#define mWarning(x) mWarningStream(x)
#endif // QT_NO_WARNING_OUTPUT

#endif

