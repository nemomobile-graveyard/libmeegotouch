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

#ifndef DUIAPPLETINVENTORYVIEW_P_H_
#define DUIAPPLETINVENTORYVIEW_P_H_

class DuiAppletInventory;
class DuiLayout;
class DuiFlowLayoutPolicy;
class DuiLinearLayoutPolicy;
class DuiButton;
class DuiOverlay;
class DuiContainer;

/*!
 * The private class for DuiAppletInventoryView.
 */
class DuiAppletInventoryViewPrivate
{
    Q_DECLARE_PUBLIC(DuiAppletInventoryView)

public:
    /*!
     * Creates an instance of DuiAppletInventoryViewPrivate.
     */
    DuiAppletInventoryViewPrivate();

    /*!
     * Destroys the DuiAppletInventoryViewPrivate.
     */
    virtual ~DuiAppletInventoryViewPrivate();

    /*!
     * Initializes the private class.
     *
     * \param controller the DuiAppletInventory for the view
     */
    void init(DuiAppletInventory *controller);

    //! The DuiAppletInventory controller
    DuiAppletInventory *controller;

    //! A layout for the inventory
    DuiLayout *layout;

    //! A linear layout policy for the inventory
    DuiLinearLayoutPolicy *layoutPolicy;

    //! A layout for the applet widgets
    DuiLayout *appletLayout;

    //! A flow layout policy for the widgets
    DuiFlowLayoutPolicy *appletLayoutPolicy;

    //! A container for the applet widgets
    DuiContainer *appletContainer;

    //! An overlay for the close button
    DuiOverlay *closeButtonOverlay;

    //! A close button
    DuiButton *closeButton;

protected:
    DuiAppletInventoryView *q_ptr;
};

#endif /* DUIAPPLETINVENTORYVIEW_P_H_ */
