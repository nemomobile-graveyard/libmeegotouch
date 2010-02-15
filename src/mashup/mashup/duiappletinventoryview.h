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

#ifndef DUIAPPLETINVENTORYVIEW_H
#define DUIAPPLETINVENTORYVIEW_H

#include <duiextendingbackgroundview.h>
#include "duiappletinventorystyle.h"
#include "duiappletinventorymodel.h"

class DuiAppletInventory;
class DuiAppletInventoryViewPrivate;
class DuiContainer;

//! \internal
/*!
 * The applet inventory view manages a layout related to the applet inventory.
 * The widgets are inside a flow layout. The view also contains a close button
 * for closing the container.
 */
class DuiAppletInventoryView : public DuiExtendingBackgroundView
{
    Q_OBJECT
    DUI_VIEW(DuiAppletInventoryModel, DuiAppletInventoryStyle)

public:
    /*!
     * Constructs a DuiAppletInventoryView.
     *
     * \param controller the DuiAppletInventory controller to be used
     */
    DuiAppletInventoryView(DuiAppletInventory *controller);

    /*!
     * Destroys the DuiAppletInventoryView.
     */
    virtual ~DuiAppletInventoryView();

private slots:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiAppletInventoryView)
    Q_DECLARE_PRIVATE(DuiAppletInventoryView)

    /*!
     * Configures various attributes of a container according to properties in a installation source widget
     * \param container the container to configure
     * \param sourceWidget the installation source widget
     */
    void connectContainerToInstallationSource(DuiContainer *container, DuiWidget *sourceWidget) const;

    //! List of applet installation source container widgets displayed on the inventory
    QList<DuiContainer *> installationSourceContainers;
};
//! \internal_end

#endif // DUIAPPLETINVENTORYVIEW_H
