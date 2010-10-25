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

#ifndef MAPPLETLOADER_H
#define MAPPLETLOADER_H

#include "mexport.h"

class QGraphicsWidget;
class MAppletMetaData;
class MDataStore;
class MDataAccess;

//! \internal
/*!
 * \brief A class that loads MeeGo Touch applets.
 */
class M_EXTENSIONS_EXPORT MAppletLoader
{
public:
    /*!
     * Loads an applet according to an applet metadata object.
     * The ownership of the returned applet widget is transferred to the caller.
     * \param metadata the metadata that is used to load the applet.
     * \param dataStore a data store object where applet instances can store their instance specific data.
     * \param settings a data store object that bundles applet settings to the applet.
     * \return the applet widget or \c NULL in an error.
     */
    static QGraphicsWidget *loadApplet(const MAppletMetaData &metadata, MDataStore &dataStore, MDataAccess &settings);

private:
    /*!
     * A private default constructor so no objects of this class can be created.
     */
    MAppletLoader();
};
//! \internal_end

#endif  // MAPPLETLOADER_H
