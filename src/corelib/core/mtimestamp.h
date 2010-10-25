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

#ifndef MTIMESTAMP_H
#define MTIMESTAMP_H

#include <QString>
#include "mexport.h"

void M_CORE_EXPORT mTimestampStream(const QString &module, const QString &file, const QString &scope, const QString &msg);

#if defined(QT_NO_WARNING_OUTPUT) || !defined(M_TIMESTAMP)
#define mTimestamp(x, msg)
#else

/**
 * Prints debug message with timestamp.
 * This function do nothing if macro M_TIMESTAMP is not defined.
 *
 * Example:
 *\code
 * #include <mtimestamp.h>
 *
 * mTimestamp("ModuleName", "Do something");
 *\endcode
 *
 */

#define mTimestamp(x, msg) mTimestampStream(x, __FILE__, __PRETTY_FUNCTION__, msg)

#endif

#endif

