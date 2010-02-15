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

#ifndef DUIOVERLAY_H
#define DUIOVERLAY_H

#include "duiscenewindow.h"
#include "duiscenewindowmodel.h"

class DuiOverlayPrivate;

/*!
 * \class DuiOverlay
 * \brief The DuiOverlay class displays a widget on top of the viewport.
 *
 * DuiOverlay class enables creation and displaying of a widget on top of the
 * current viewport. It provides a widget which is ready for use out of the box.
 * It follows the idea used for DuiApplication's centralWidget(), i.e. it can be
 * used as a panel (can have a layout applied and filled with other widgets) or
 * simply substituted with another widget. The widget can be positioned inside
 * the scene using alignment and offset defined in stylesheet.
 *
 * \sa DuiApplicationPage
 */
class DUI_EXPORT DuiOverlay : public DuiSceneWindow
{
    Q_OBJECT
    DUI_CONTROLLER(DuiSceneWindow)

public:

    /*!
     * \brief Constructs a widget with the given \a parent.
     */
    DuiOverlay(QGraphicsItem *parent = 0);

    /*!
     * \brief Destructor.
     */
    virtual ~DuiOverlay();

    /*!
     * \brief Returns a pointer to the topmost widget. By default this class
     *        provides a widget (panel) on which other widgets can be placed.
     */
    DuiWidget *widget() const;

    /*!
     * \brief Sets the given \a widget as a topmost widget. The ownership
     *        of the \a widget is passed to the DuiOverlay instance.
     * \param widget A widget to be set as topmost.
     */
    void setWidget(DuiWidget *widget);

private:

    Q_DECLARE_PRIVATE(DuiOverlay)
    Q_DISABLE_COPY(DuiOverlay)

    friend class DuiOverlayView;
#ifdef UNIT_TEST
    friend class Ut_DuiOverlay;
#endif
};

#endif
