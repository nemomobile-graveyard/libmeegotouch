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

#ifndef MAPPLETINVENTORYVIEW_H
#define MAPPLETINVENTORYVIEW_H

#include <mextendingbackgroundview.h>
#include "mappletinventorystyle.h"
#include "mappletinventorymodel.h"

class MAppletInventory;
class MAppletInventoryViewPrivate;
class MContainer;

//! \internal
/*!
 * The applet inventory view manages a layout related to the applet inventory.
 * The widgets are inside a flow layout. The view also contains a close button
 * for closing the container.
 */
class MAppletInventoryView : public MExtendingBackgroundView
{
    Q_OBJECT
    M_VIEW(MAppletInventoryModel, MAppletInventoryStyle)

public:
    /*!
     * Constructs a MAppletInventoryView.
     *
     * \param controller the MAppletInventory controller to be used
     */
    MAppletInventoryView(MAppletInventory *controller);

    /*!
     * Destroys the MAppletInventoryView.
     */
    virtual ~MAppletInventoryView();

private slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(MAppletInventoryView)
    Q_DECLARE_PRIVATE(MAppletInventoryView)

    /*!
     * Configures various attributes of a container according to properties in a installation source widget
     * \param container the container to configure
     * \param sourceWidget the installation source widget
     */
    void connectContainerToInstallationSource(MContainer *container, MWidget *sourceWidget) const;

    //! List of applet installation source container widgets displayed on the inventory
    QList<MContainer *> installationSourceContainers;

protected:
    // FIXME this class shouldn't have its own private class pointer. Instead
    // it should use the one in the class(es) this class inherits from.
    MAppletInventoryViewPrivate *const d_ptr;
};
//! \internal_end

#endif // MAPPLETINVENTORYVIEW_H
