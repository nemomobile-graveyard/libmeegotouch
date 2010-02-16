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

#ifndef DUIMASHUPCANVASVIEW_P_H_
#define DUIMASHUPCANVASVIEW_P_H_

#include "private/duiwidgetview_p.h"

class DuiLayout;
class DuiFlowLayoutPolicy;
class QGraphicsLinearLayout;
class DuiButton;
class DuiAppletInventory;
class DuiPannableViewport;
class DuiSceneWindow;

/*!
 * DuiMashupCanvasViewPrivate is the private class for DuiMashupCanvasView.
 */
class DuiMashupCanvasViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiMashupCanvasView)

public:
    /*!
     * Constructs a DuiMashupCanvasViewPrivate private class for
     * DuiMashupCanvasView.
     */
    DuiMashupCanvasViewPrivate();

    /*!
     * Destroys the DuiMashupCanvasViewPrivate.
     */
    virtual ~DuiMashupCanvasViewPrivate();

    /*!
     * Updates the geometry of each widget present in the layout into the data
     * store.
     */
    void updateAppletData();

    /*!
     * Removes those widgets from the layout that are not present in the data
     * store and adds those widgets to the layout which are present in the data
     * store but not yet in the layout.
     */
    void updateAppletLayout();

    /*!
     * Used to check properties of applet and connect signals between applet and DuiContainer
     */
    void connectContainerToApplet(DuiContainer *container, DuiWidget *applet) const;

    /*!
     * Creates a new container and inserts the given \p widget into the container. 
     * Sets up the container for the \p widget.
     * \return Created container.
     * \param widget Widget to be inserted into the container.
     */
    DuiContainer* createWidgetContainer(DuiWidget* widget) const;

    /*!
     * Sets up containers in this mashup canvas view. If the \p enabled is \c true the 
     * containers are made visible for all widgets in this mashup view. 
     * If the \p enabled is \c false all containers hidden in this mashup canvas
     *
     * \param enabled Whether containers are enabled or not.
     */
    void setupContainers(bool enabled);

    /*!
     * Toggles if the given container is visible or not. If the \p enabled is \c true the 
     * containers are set to visible, otherwise they are hidden.
     *
     * \param container The container whos visibility to modify
     * \param enabled Whether containers are enabled or not.
     */
    void setContainerEnabled(DuiContainer* container, bool enabled);

    //! The DuiMashupCanvas controller.
    DuiMashupCanvas *controller;

    //! The main layout of the mashup canvas.
    QGraphicsLinearLayout *mainLayout;

    //! Layout used to layout the applet instances on this canvas.
    DuiLayout *appletLayout;

    //! Layout policy used to layout the applet instances on this canvas.
    DuiFlowLayoutPolicy *layoutPolicy;

    //! Applet inventory button
    DuiButton *appletInventoryButton;

    //! Applet inventory for this mashup
    DuiAppletInventory *appletInventory;

    //! DuiSceneWindow for the applet inventory
    DuiSceneWindow *appletInventoryWindow;

    //! Pannable viewport in which the applet inventory is displayed
    DuiPannableViewport *appletInventoryViewport;


};

#endif /* DUIMASHUPCANVASVIEW_P_H_ */
