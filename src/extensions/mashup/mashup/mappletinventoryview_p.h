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

#ifndef MAPPLETINVENTORYVIEW_P_H_
#define MAPPLETINVENTORYVIEW_P_H_

class MAppletInventory;
class MLayout;
class MFlowLayoutPolicy;
class MLinearLayoutPolicy;
class MButton;
class MOverlay;
class MContainer;

/*!
 * The private class for MAppletInventoryView.
 */
class MAppletInventoryViewPrivate
{
    Q_DECLARE_PUBLIC(MAppletInventoryView)

public:
    /*!
     * Creates an instance of MAppletInventoryViewPrivate.
     */
    MAppletInventoryViewPrivate();

    /*!
     * Destroys the MAppletInventoryViewPrivate.
     */
    virtual ~MAppletInventoryViewPrivate();

    /*!
     * Initializes the private class.
     *
     * \param controller the MAppletInventory for the view
     */
    void init(MAppletInventory *controller);

    //! The MAppletInventory controller
    MAppletInventory *controller;

    //! A layout for the inventory
    MLayout *layout;

    //! A linear layout policy for the inventory
    MLinearLayoutPolicy *layoutPolicy;

    //! A layout for the applet widgets
    MLayout *appletLayout;

    //! A flow layout policy for the widgets
    MFlowLayoutPolicy *appletLayoutPolicy;

    //! A container for the applet widgets
    MContainer *appletContainer;

    //! An overlay for the close button
    MOverlay *closeButtonOverlay;

    //! A close button
    MButton *closeButton;

protected:
    MAppletInventoryView *q_ptr;
};

#endif /* MAPPLETINVENTORYVIEW_P_H_ */
