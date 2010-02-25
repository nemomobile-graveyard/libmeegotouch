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

#ifndef DUIAPPLICATIONEXTENSIONMANAGER_H
#define DUIAPPLICATIONEXTENSIONMANAGER_H

#include <QObject>

class DuiDataStore;

class DuiApplicationExtensionManager : public QObject
{
    Q_OBJECT

public:
    /*
     * Constructs a  DuiApplicationExtensionManager.
     * \param identifierAn identifier to identify this DuiApplicationExtensionManager in this applications context.
     * \param dataStore . Perhaps not used for now ?
     */
    DuiApplicationExtensionManager(const QString &identifier, DuiDataStore *dataStore = NULL);

    //! Destructs the DuiApplicationExtensionManager object
   virtual  ~DuiApplicationExtensionManager();
};
#endif // DUIAPPLICATIONEXTENSIONMANAGER_H
