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

#ifndef MTOOLBARTABVIEW_H
#define MTOOLBARTABVIEW_H

#include "mtoolbarview.h"
#include <mwidgetmodel.h>
#include <mtoolbartabstyle.h>
#include <mnamespace.h>

class MToolBar;
class MToolbarTabViewPrivate;

/*!
    \class MToolbarTabView
    \brief View class for MToolBar widget when it is used in tab mode.

    \ingroup views

    \section MToolbarTabViewOverview Overview
        MToolbarTabView is used to visualize mutually exclusive latch-down type of buttons
        placed into the MToolBar.

        The outlook of toolbar can be changed using the styling attributes defined
        in MToolbarStyle and MWidgetStyle.

    \section MToolBarTabInteractions Interactions
        - The toolbar has currently no interactions by itself: the interactions are
          determined by the UI controls that are placed inside the toolbar.
        - The toolbar can be affected by possible interactions of going to and from the
          full screen mode; this would set the visibility of the toolbar on and off.
        - Setting the visibility of the toolbar on/off should be accompanied by a
          (sliding) transition, hiding and revealing the toolbar.

    \sa MToolBar MToolbarTabStyle
*/

class M_VIEWS_EXPORT MToolbarTabView : public MToolBarView
{
    Q_OBJECT
    M_VIEW(MWidgetModel, MToolbarTabStyle)
    Q_DECLARE_PRIVATE(MToolbarTabView)

public:

    /*!
        \brief Constructs toolbar tab view.
        \param Pointer to the controller.
     */
    MToolbarTabView(MToolBar *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~MToolbarTabView();

protected:
    /*!
        \brief protected constructor
        \param dd Shared private class
        \param controller The controller associated with the view.
    */
    MToolbarTabView(MToolbarTabViewPrivate &dd, MToolBar *controller);

private:
    Q_DISABLE_COPY(MToolbarTabView)

#ifdef UNIT_TEST
    friend class Ut_MToolbarTabView;
#endif

};

#endif
