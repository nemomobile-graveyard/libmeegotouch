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

#ifndef DUIAPPLICATIONEXTENSIONAREAVIEW_P_H_
#define DUIAPPLICATIONEXTENSIONAREAVIEW_P_H_

#include "applicationextension/duiextensionareaview_p.h"

class DuiLayout;
class DuiLinearLayoutPolicy;
class QGraphicsLinearLayout;
class DuiButton;
class DuiApplicationExtensionArea;
class DuiWidget;

/*!
 * DuiApplicationExtensionAreaViewPrivate is the private class for DuiApplicationExtensionAreaView.
 */
class DuiApplicationExtensionAreaViewPrivate : public DuiExtensionAreaViewPrivate
{
    Q_DECLARE_PUBLIC(DuiApplicationExtensionAreaView)

public:
    /*!
     * Constructs a DuiApplicationExtensionAreaViewPrivate private class for
     * DuiApplicationExtensionAreaView.
     */
    DuiApplicationExtensionAreaViewPrivate();

    /*!
     * Destroys the DuiApplicationExtensionAreaViewPrivate.
     */
    virtual ~DuiApplicationExtensionAreaViewPrivate();

    /*!
     * Initializes this class. This method should be called before this class
     * is used for anything else.
     */
    void init(DuiApplicationExtensionArea *controller);

    //! The DuiApplicationExtensionArea controller.
    DuiApplicationExtensionArea *controller;

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
};

#endif /* DUIAPPLICATIONEXTENSIONAREAVIEW_P_H_ */
