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
#include "applicationextension/duiextensionareaview_p.h"
#include "duimashupcanvasview.h"

class DuiLayout;
class DuiFlowLayoutPolicy;
class QGraphicsLinearLayout;
class DuiButton;
class DuiAppletInventory;
class DuiPannableViewport;
class DuiSceneWindow;
class DuiMashupCanvas;

/*!
 * DuiMashupCanvasViewPrivate is the private class for DuiMashupCanvasView.
 */
class DuiMashupCanvasViewPrivate : public DuiExtensionAreaViewPrivate
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
     * Initializes the class
     */
    void init();

    /*!
     * Adds a widget to the layout
     * \param widget the widget
     * \param index the index to insert or -1 to add to the end
     */
    virtual void addToLayout(DuiWidget *widget, int index = -1);

    /*!
     * Used to check properties of widget and connect signals between it and the DuiContainer
     */
    virtual void connectContainerToWidget(DuiContainer *container, DuiWidget *widget) const;

    //! The DuiMashupCanvas controller.
    DuiMashupCanvas *controller;

    //! The main layout of the mashup canvas.
    QGraphicsLinearLayout *mainLayout;

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
