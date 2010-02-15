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

#include "widgetsgalleryretranslator.h"

#include <QDebug>

#include "duigconfitem.h"
#include "duilocale.h"

void WidgetsgalleryRetranslator::widgetsgalleryRetranslate()
{
    qDebug() << __PRETTY_FUNCTION__;
    DuiGConfItem languageItem("/Dui/i18n/Language");
    QString language = languageItem.value().toString();
    DuiLocale locale(language);
    // installs the catalog for the “Engineering English”:
    locale.installTrCatalog("libdui.qm");
    // installs the catalog for the “Engineering English”:
    locale.installTrCatalog("widgetsgallery.qm");
    // installs the catalog for the real translation:
    locale.installTrCatalog("libdui");
    // installs the catalog for the real translation:
    locale.installTrCatalog("common");
    // installs the catalog for the real translation:
    locale.installTrCatalog("widgetsgallery");
    DuiLocale::setDefault(locale);
}
