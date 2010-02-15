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

#ifndef DUIAPPLETINSTANCEDATA_H_
#define DUIAPPLETINSTANCEDATA_H_

#include <QString>
#include "duiappletid.h"

class DuiWidget;
class DuiFileDataStore;
class DuiSubDataStore;
class DuiDataAccess;
class DuiDataStore;
class DuiAppletHandle;

//! \internal

/*!
 * A helper class for storing data about the applet instances.
 */
class DuiAppletInstanceData
{
public:
    enum InstallationStatus {
        Unknown,
        PlaceHolderRequired,
        Installing,
        Installed
    };

    /*!
     * Constructs a new data object.
     */
    DuiAppletInstanceData();

    /*!
     * Destructor.
     */
    virtual ~DuiAppletInstanceData();

    //! The id of the applet instance.
    DuiAppletId appletId;
    //! The desktop file of the applet.
    QString desktopFile;
    //! The name of the package the applet is being installed from.
    QString packageName;
    //! The title of the applet
    QString title;
    //! The size of the applet in landscape
    QString sizeLandscape;
    //! The size of the applet in portrait
    QString sizePortrait;
    //! The applet widget.
    DuiWidget *widget;
    //! The path where applet instances can store their instance data.
    QString instanceDataFilePath;
    //! The data store where applet instances can store their instance data.
    DuiFileDataStore *instanceDataStore;
    //! A data store that the mashup canvas can use to store layout etc. data about this applet instance.
    DuiSubDataStore *mashupCanvasDataStore;
    //! An interface for the applets to access their settings.
    DuiDataAccess *settingsDataAccess;
    //! Applet installation status
    InstallationStatus installationStatus;
};

//! \internal_end

#endif /* DUIAPPLETINSTANCEDATA_H_ */
