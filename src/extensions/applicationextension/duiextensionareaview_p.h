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

#ifndef DUIEXTENSIONAREAVIEW_P_H_
#define DUIEXTENSIONAREAVIEW_P_H_

#include "applicationextension/duiextensionareaview.h"

class DuiContainer;

/*!
 * DuiExtensionAreaViewPrivate is the private class for DuiExtensionAreaView.
 */
class DuiExtensionAreaViewPrivate
{
    Q_DECLARE_PUBLIC(DuiExtensionAreaView)

public:
    /*!
     * Constructs a DuiExtensionAreaViewPrivate private class for
     * DuiExtensionAreaView.
     */
    DuiExtensionAreaViewPrivate();

    /*!
     * Destroys the DuiExtensionAreaViewPrivate.
     */
    virtual ~DuiExtensionAreaViewPrivate();

    /*!
     * pointer to controller
     */
    DuiWidgetController     *controller;

    /*!
     * Updates the geometry of each widget present in the layout into the data
     * store.
     */
    void updateData();

    /*!
     * Removes those widgets from the layout that are not present in the data
     * store and adds those widgets to the layout which are present in the data
     * store but not yet in the layout.
     */
    void updateLayout();

    /*!
     * Creates a new container and inserts the given \p widget into the container.
     * Sets up the container for the \p widget.
     * \return Created container.
     * \param widget Widget to be inserted into the container.
     */
    DuiContainer *createWidgetContainer(QGraphicsWidget *widget) const;

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
    void setContainerEnabled(DuiContainer *container, bool enabled);

    /*!
     * Used to check properties of widget and connect signals between it and the DuiContainer
     */
    virtual void connectContainerToWidget(DuiContainer *container, QGraphicsWidget *widget) const;

    /*!
     * Adds a widget to the layout
     * \param widget the widget
     * \param index the index to insert or -1 to add to the end
     */
    virtual void addToLayout(DuiWidget *widget, int index = -1);

    //! Layout used to layout the widget instances on this extension area.
    QGraphicsLayout *layout;

protected:
    DuiExtensionAreaView *q_ptr;
};

#endif /* DUIEXTENSIONAREAVIEW_P_H_ */
