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

#ifndef MAPPLICATIONEXTENSIONAREA_P_H
#define MAPPLICATIONEXTENSIONAREA_P_H

#include "mextensionarea_p.h"
#include "mapplicationextensionarea.h"
#include <QSharedPointer>

class MApplicationExtensionManager;
class MApplicationExtensionInterface;

/*!
 * Private class for MApplicationExtensionArea.
 */
class MApplicationExtensionAreaPrivate : public MExtensionAreaPrivate
{
    Q_DECLARE_PUBLIC(MApplicationExtensionArea)

public:
    /*!
     * Constructor.
     */
    MApplicationExtensionAreaPrivate();

    /*!
     * Destructor.
     */
    virtual ~MApplicationExtensionAreaPrivate();

    /*!
     * Initializes this class. This method should be called before this class
     * is used for anything else.
     *
     * \param interface the extension interface name for the application extension area.
     */
    void init(const QString &interface, Qt::ConnectionType connectionType = Qt::AutoConnection);

   /*!
    * Returns a list of in process extensions loaded by this manager
    *
    * \return List of application extension interface pointers. Receving party should appropriately cast them correctly.
    */
    QList<MApplicationExtensionInterface*> extensions();

    //! Application extension manager
    QSharedPointer<MApplicationExtensionManager> extensionManager;
};

#endif // MAPPLICATIONEXTENSIONAREA_P_H
