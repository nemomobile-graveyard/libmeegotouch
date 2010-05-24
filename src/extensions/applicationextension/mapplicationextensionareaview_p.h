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

#ifndef MAPPLICATIONEXTENSIONAREAVIEW_P_H_
#define MAPPLICATIONEXTENSIONAREAVIEW_P_H_

#include "applicationextension/mextensionareaview_p.h"

class MLayout;
class MLinearLayoutPolicy;
class QGraphicsLinearLayout;
class MButton;
class MApplicationExtensionArea;
class QGraphicsWidget;

/*!
 * MApplicationExtensionAreaViewPrivate is the private class for MApplicationExtensionAreaView.
 */
class MApplicationExtensionAreaViewPrivate : public MExtensionAreaViewPrivate
{
    Q_DECLARE_PUBLIC(MApplicationExtensionAreaView)

public:
    /*!
     * Constructs a MApplicationExtensionAreaViewPrivate private class for
     * MApplicationExtensionAreaView.
     */
    MApplicationExtensionAreaViewPrivate();

    /*!
     * Destroys the MApplicationExtensionAreaViewPrivate.
     */
    virtual ~MApplicationExtensionAreaViewPrivate();

    /*!
     * Initializes this class. This method should be called before this class
     * is used for anything else.
     */
    void init(MApplicationExtensionArea *controller);

    //! The MApplicationExtensionArea controller.
    MApplicationExtensionArea *controller;

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
};

#endif /* MAPPLICATIONEXTENSIONAREAVIEW_P_H_ */
