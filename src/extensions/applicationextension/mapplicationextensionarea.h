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

#ifndef MAPPLICATIONEXTENSIONAREA_H
#define MAPPLICATIONEXTENSIONAREA_H

#include "mextensionarea.h"
#include "mapplicationextensionareamodel.h"

class MApplicationExtensionAreaPrivate;
class MApplicationExtensionInterface;

/*!
 * MApplicationExtensionArea is a widget which can be populated with application extensions. MApplicationExtensionArea
 * can be placed on any view that wants to have application extension support.
 *
 * The MApplicationExtensionArea will load application extensions which implement the specified interface.
 *
 * \note The behaviour of this class will change in the future and cannot be used for real
 * application extension development right now.
 *
 * \see \ref applicationextensions
 */
class M_EXPORT MApplicationExtensionArea : public MExtensionArea
{
    Q_OBJECT
    M_CONTROLLER(MApplicationExtensionArea)

public:
    /*!
     * Constructor
     * \param interface Specifies which interface the application extensions must implement in order to be loaded to
     * this application extension area.
     * \param enableInProcessExtensions \c true if in process extensions should be loaded. \c false otherwise
     * \param parent Optional Object's parent
     */
    explicit MApplicationExtensionArea(const QString &interface, const bool enableInProcessExtensions = true, QGraphicsItem *parent = NULL);

    /*!
     * Returns a list of in process extensions loaded by this manager
     *
     * \return List of application extension interface pointers. Receving party should appropriately cast them.
     */
    QList<MApplicationExtensionInterface*> extensions();

    /*!
     * Destructor
     */
    virtual ~MApplicationExtensionArea();

signals:
    /*!
     * Signal sent when an extension has been instantiated.
     *
     * \param extension the extension that was instantiated
     */
    void extensionInstantiated(MApplicationExtensionInterface *extension);

    /*!
     * Signal sent when an extension has been removed.
     *
     * \param extension the extension that was removed
     */
    void extensionRemoved(MApplicationExtensionInterface* extension);

protected:
    /*!
     * Protected constructor to be called by derived classes to set up the private implementation
     * hierarchy.
     */
    MApplicationExtensionArea(MApplicationExtensionAreaPrivate *dd, MApplicationExtensionAreaModel *model,
                                QGraphicsItem *parent, const QString &interface, const bool enableInProcessExtensions);

private:
    Q_DECLARE_PRIVATE(MApplicationExtensionArea)
    Q_DISABLE_COPY(MApplicationExtensionArea)
};

#endif // MAPPLICATIONEXTENSIONAREA_H
