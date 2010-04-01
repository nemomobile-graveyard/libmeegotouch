/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUITIMESTAMP_H
#define DUITIMESTAMP_H

#include <QString>
#include "duiexport.h"

void DUI_EXPORT duiTimestampStream(const QString &module, const QString &file, const QString &scope, const QString &msg);

#if defined(QT_NO_WARNING_OUTPUT) || !defined(DUI_TIMESTAMP)
#define duiTimestamp(x, msg)
#else

/**
 * Prints debug message with timestamp.
 * This function do nothing if macro DUI_TIMESTAMP is not defined.
 *
 * Example:
 *\code
 * #include <duitimestamp.h>
 *
 * duiTimestamp("ModuleName", "Do something");
 *\endcode
 *
 */

#define duiTimestamp(x, msg) duiTimestampStream(x, __FILE__, __PRETTY_FUNCTION__, msg)

#endif

#endif

