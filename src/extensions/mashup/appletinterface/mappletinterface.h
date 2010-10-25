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

#ifndef MAPPLETINTERFACE_H
#define MAPPLETINTERFACE_H

#include <QtPlugin>
#include "mexport.h"

class QGraphicsWidget;
class MAppletMetaData;
class MDataStore;
class MDataAccess;

/**
 * MAppletInterface is the base class for desktop applet
 * entry-point classes.
 *
 * Applet developers need to implement this interface in their applet binary
 * and export their implementation from the binary. The host application will maintain
 * an inventory of available applets and instantiate them using this interface
 * when needed.
 *
 * \see \ref appletdevelopment
 */
class M_EXTENSIONS_EXPORT MAppletInterface
{
public:
    /*!
     * Destructor.
     */
    virtual ~MAppletInterface() {}

    /**
     * This method is called to construct a new applet widget instance.
     * Caller will maintain the ownership of the constructed widget and
     * will destroy it when it is no longer needed.
     * \param metadata Is const reference to metadata of the applet.
     * \param instanceData A data store object that should be used to store permanent data of the constructed applet instance.
     * \param settings A data store object that bundles applet settings to the applet. Contains both the
     *                 instance settings and the global settings.
     * \return New widget instance that visualizes this applet.
     */
    virtual QGraphicsWidget *constructWidget(const MAppletMetaData &metadata, MDataStore &instanceData, MDataAccess &settings) = 0;
};

Q_DECLARE_INTERFACE(MAppletInterface, "com.meego.core.MAppletInterface/1.0")

#endif // MAPPLETINTERFACE_H
