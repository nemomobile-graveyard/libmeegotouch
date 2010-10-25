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

#ifndef MAPPLICATIONEXTENSIONINTERFACE_H
#define MAPPLICATIONEXTENSIONINTERFACE_H

#include <QtPlugin>
#include "mexport.h"

class QGraphicsWidget;

/**
 * MApplicationExtensionInterface is the base class for application extension
 * entrypoint classes.
 *
 * Application developers who want to provide application extension support
 * need to derive their application specific extension interface from this
 * interface if they need some application specific functionality to be
 * implemented by the extensions.
 *
 * Application extension developers need to implement the interface derived
 * from this interface in their application extension binary and export their
 * implementation from the binary. The host application will maintain an
 * inventory of available extensions and instantiate them using this interface
 * when needed.
 *
 * \see \ref applicationextensions
 */
class M_EXTENSIONS_EXPORT MApplicationExtensionInterface
{
public:
    /*!
     * Destructor.
     */
    virtual ~MApplicationExtensionInterface() {}

    /**
     * This method is called to initialize the application extension.
     *
     * \param interface the name of the interface being initialized. The extension can but does not need to use this information for anything.
     * \return \c true if the initialization succeeded, \c false otherwise.
     */
    virtual bool initialize(const QString &interface) = 0;

    /**
     * Returns a widget representing the application extension (if any).
     * The extension will retain ownership of the widget.
     *
     * \return a pointer to the extension's widget or NULL if the extension does not have a widget.
     */
    virtual QGraphicsWidget *widget() { return NULL; }
};

Q_DECLARE_INTERFACE(MApplicationExtensionInterface, "com.meego.core.MApplicationExtensionInterface/1.0")

#endif // MAPPLICATIONEXTENSIONINTERFACE_H
