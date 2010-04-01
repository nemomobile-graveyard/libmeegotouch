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

#ifndef DUITOOLBARTABVIEW_H
#define DUITOOLBARTABVIEW_H

#include "duiwidgetview.h"
#include <duiwidgetmodel.h>
#include <duitoolbartabstyle.h>
#include <duinamespace.h>

class DuiToolBar;
class DuiToolbarTabViewPrivate;

/*!
    \class DuiToolbarTabView
    \brief View class for DuiToolBar widget when it is used in tab mode.

    \ingroup views

    \section DuiToolbarTabViewOverview Overview
        DuiToolbarTabView is used to visualize mutually exclusive latch-down type of buttons
        placed into the DuiToolBar.

        The outlook of toolbar can be changed using the styling attributes defined
        in DuiToolbarStyle and DuiWidgetStyle.

    \section DuiToolBarTabInteractions Interactions
        - The toolbar has currently no interactions by itself: the interactions are
          determined by the UI controls that are placed inside the toolbar.
        - The toolbar can be affected by possible interactions of going to and from the
          full screen mode; this would set the visibility of the toolbar on and off.
        - Setting the visibility of the toolbar on/off should be accompanied by a
          (sliding) transition, hiding and revealing the toolbar.

    \sa DuiToolBar DuiToolbarTabStyle
*/

class DUI_EXPORT DuiToolbarTabView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiWidgetModel, DuiToolbarTabStyle)

public:

    /*!
        \brief Constructs toolbar tab view.
        \param Pointer to the controller.
     */
    DuiToolbarTabView(DuiToolBar *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~DuiToolbarTabView();

protected:
    /*!
        \brief protected constructor
        \param dd Shared private class
        \param controller The controller associated with the view.
    */
    DuiToolbarTabView(DuiToolbarTabViewPrivate &dd, DuiToolBar *controller);
    DuiToolbarTabViewPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(DuiToolbarTabView)
    Q_DECLARE_PRIVATE(DuiToolbarTabView)

    Q_PRIVATE_SLOT(d_func(), void _q_groupButtonClicked(bool))
    Q_PRIVATE_SLOT(d_func(), void _q_groupActionToggled(bool))
#ifdef UNIT_TEST
    friend class Ut_DuiToolbarTabView;
#endif

};

#endif
