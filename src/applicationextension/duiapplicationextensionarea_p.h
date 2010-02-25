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

#ifndef DUIAPPLICATIONEXTENSIONAREA_P_H
#define DUIAPPLICATIONEXTENSIONAREA_P_H

#include "duiextensionarea_p.h"
#include "duiapplicationextensionarea.h"
#include <QSharedPointer>

class DuiApplicationExtensionManager;
class DuiApplicationExtensionInterface;

/*!
 * Private class for DuiApplicationExtensionArea.
 */
class DuiApplicationExtensionAreaPrivate : public DuiExtensionAreaPrivate
{
    Q_DECLARE_PUBLIC(DuiApplicationExtensionArea)

public:
    /*!
     * Constructor.
     */
    DuiApplicationExtensionAreaPrivate();

    /*!
     * Destructor.
     */
    virtual ~DuiApplicationExtensionAreaPrivate();

    /*!
     * Initializes this class. This method should be called before this class
     * is used for anything else.
     *
     * \param interface the extension interface name for the application extension area.
     * \param enableInProcessExtensions \c true if in process extensions should be loaded. \c false otherwise
     */
    void init(const QString &interface, const bool enableInProcessExtensions);

   /*!
    * Returns a list of in process extensions loaded by this manager
    *
    * \return List of application extension interface pointers. Receving party should appropriately cast them correctly.
    */
    QList<DuiApplicationExtensionInterface*> extensions();

    //! Application extension manager
    QSharedPointer<DuiApplicationExtensionManager> extensionManager;
};

#endif // DUIAPPLICATIONEXTENSIONAREA_P_H
