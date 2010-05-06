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

#include "widgetsgalleryretranslator.h"

#include <QDebug>

#include "mgconfitem.h"
#include "mlocale.h"

void WidgetsgalleryRetranslator::widgetsgalleryRetranslate()
{
    qDebug() << __PRETTY_FUNCTION__;
    // When the gconf key /meegotouch/i18n/language specifying the system
    // language changes, the system default locale of a MApplication
    // is changed automatically. All translation catalogs are reloaded
    // according to the new locale settings. Therefore, in most cases,
    // calling a slot like this one here is not needed, but it is
    // possible to use it it to do something unusual when the language
    // changes.
}
