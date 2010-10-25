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

#ifndef MOVERLAY_H
#define MOVERLAY_H

#include "mscenewindow.h"
#include "mscenewindowmodel.h"

class MOverlayPrivate;

/*!
 * \class MOverlay
 * \brief The MOverlay class displays a widget on top of the viewport.
 *
 * MOverlay class enables creation and displaying of a widget on top of the
 * current viewport. It provides a widget which is ready for use out of the box.
 * It follows the idea used for MApplication's centralWidget(), i.e. it can be
 * used as a panel (can have a layout applied and filled with other widgets) or
 * simply substituted with another widget. The widget can be positioned inside
 * the scene using alignment and offset defined in stylesheet.
 *
 * \sa MApplicationPage
 */
class M_CORE_EXPORT MOverlay : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MSceneWindow)

public:

    /*!
     * \brief Constructs a widget with the given \a parent.
     */
    MOverlay(QGraphicsItem *parent = 0);

    /*!
     * \brief Destructor.
     */
    virtual ~MOverlay();

    /*!
     * \brief Returns a pointer to the topmost widget. By default this class
     *        provides a widget (panel) on which other widgets can be placed.
     */
    MWidget *widget() const;

    /*!
     * \brief Sets the given \a widget as a topmost widget. The ownership
     *        of the \a widget is passed to the MOverlay instance.
     * \param widget A widget to be set as topmost.
     */
    void setWidget(MWidget *widget);

private:

    Q_DECLARE_PRIVATE(MOverlay)
    Q_DISABLE_COPY(MOverlay)

    friend class MOverlayView;
#ifdef UNIT_TEST
    friend class Ut_MOverlay;
#endif
};

#endif
