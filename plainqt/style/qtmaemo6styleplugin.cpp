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

#include "qtmaemo6styleplugin.h"

#include "qtmaemo6style.h"

QStringList QtMaemo6StylePlugin::keys() const
{
    return QStringList()
           << "maemo6"
           << "MBasicQtStyle"
           << "meegotouch"
           << "bqs";
}

QStyle *QtMaemo6StylePlugin::create(const QString &key)
{
    QString lkey = key.toLower();

    if ((lkey == "mbasicqtstyle") || (lkey == "bqs"))
        return new QtMaemo6TestStyle;

    if ((lkey == "maemo6") || (lkey == "meegotouch"))
        return new QtMaemo6Style;

    return 0;
}

Q_EXPORT_PLUGIN2(pnp_qtmaemo6styleplugin, QtMaemo6StylePlugin)
