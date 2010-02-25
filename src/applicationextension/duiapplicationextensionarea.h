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

#ifndef DUIAPPLICATIONEXTENSIONAREA_H
#define DUIAPPLICATIONEXTENSIONAREA_H

#include "duiextensionarea.h"
#include "duiapplicationextensionareamodel.h"

class DuiApplicationExtensionAreaPrivate;

/*!
 * DuiApplicationExtensionArea is a widget which can be populated with application extensions. DuiApplicationExtensionArea
 * can be placed on any view that wants to have application extension support.
 *
 * The DuiApplicationExtensionArea will load application extensions which implement the specified interface.
 *
 * \note The behaviour of this class will change in the future and cannot be used for real
 * application extension development right now.
 *
 * \see \ref applicationextensions
 */
class DUI_EXPORT DuiApplicationExtensionArea : public DuiExtensionArea
{
    Q_OBJECT
    DUI_CONTROLLER(DuiApplicationExtensionArea)

public:
    /*!
     * Constructor
     * \param interface Specifies which interface the application extensions must implement in order to be loaded to
     * this application extension area.
     * \param parent Optional Object's parent
     */
    explicit DuiApplicationExtensionArea(const QString &interface, QGraphicsItem *parent = NULL);

    /*!
     * Destructor
     */
    virtual ~DuiApplicationExtensionArea();

protected:
    /*!
     * Protected constructor to be called by derived classes to set up the private implementation
     * hierarchy.
     */
    DuiApplicationExtensionArea(DuiApplicationExtensionAreaPrivate *dd, DuiApplicationExtensionAreaModel *model,
                                QGraphicsItem *parent, const QString &interface);

private:
    Q_DECLARE_PRIVATE(DuiApplicationExtensionArea)
    Q_DISABLE_COPY(DuiApplicationExtensionArea)
};

#endif // DUIAPPLICATIONEXTENSIONAREA_H
