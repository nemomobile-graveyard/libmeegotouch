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

#ifndef DUIGCONFDATASTORE_P_H
#define DUIGCONFDATASTORE_P_H

class DuiGConfDataStorePrivate
{
public:
    DuiGConfDataStorePrivate(const QString &keyPath);

    //! Type for a container that maps strings to GConf items.
    typedef QHash<QString, DuiGConfItem *> KeyContainer;

    //! A container for the GConf items.
    KeyContainer keyContainer;

    //! The gconf path of this datastore
    QString keyPath;
};

#endif  /* DUIGCONFDATASTORE_P_H */
