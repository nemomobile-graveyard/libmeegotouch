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

#ifndef MMASHUPCANVASVIEW_P_H_
#define MMASHUPCANVASVIEW_P_H_

#include "mwidgetview_p.h"
#include "mextensionareaview_p.h"
#include "mmashupcanvasview.h"

class MLayout;
class MFlowLayoutPolicy;
class QGraphicsLinearLayout;
class MButton;
class MAppletInventory;
class MPannableViewport;
class MSceneWindow;
class MMashupCanvas;

/*!
 * MMashupCanvasViewPrivate is the private class for MMashupCanvasView.
 */
class MMashupCanvasViewPrivate : public MExtensionAreaViewPrivate
{
    Q_DECLARE_PUBLIC(MMashupCanvasView)

public:
    /*!
     * Constructs a MMashupCanvasViewPrivate private class for
     * MMashupCanvasView.
     */
    MMashupCanvasViewPrivate();

    /*!
     * Destroys the MMashupCanvasViewPrivate.
     */
    virtual ~MMashupCanvasViewPrivate();

    /*!
     * Initializes the class
     */
    void init();

    /*!
     * Adds a widget to the layout
     * \param widget the widget
     * \param index the index to insert or -1 to add to the end
     */
    virtual void addToLayout(QGraphicsWidget *widget, int index = -1);

    /*!
     * Used to check properties of widget and connect signals between it and the MContainer
     */
    virtual void connectContainerToWidget(MContainer *container, QGraphicsWidget *widget) const;

    //! The MMashupCanvas controller.
    MMashupCanvas *controller;

    //! The main layout of the mashup canvas.
    QGraphicsLinearLayout *mainLayout;

    //! Layout policy used to layout the applet instances on this canvas.
    MFlowLayoutPolicy *layoutPolicy;

    //! Applet inventory button
    MButton *appletInventoryButton;

    //! Applet inventory for this mashup
    MAppletInventory *appletInventory;

    //! MSceneWindow for the applet inventory
    MSceneWindow *appletInventoryWindow;

    //! Pannable viewport in which the applet inventory is displayed
    MPannableViewport *appletInventoryViewport;
};

#endif /* MMASHUPCANVASVIEW_P_H_ */
