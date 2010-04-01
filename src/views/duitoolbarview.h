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

#ifndef DUITOOLBARVIEW_H
#define DUITOOLBARVIEW_H

#include "duiwidgetview.h"
#include <duiwidgetmodel.h>
#include <duitoolbarstyle.h>
#include <duinamespace.h>

class DuiToolBar;
class DuiToolBarViewPrivate;

/*!
    \class DuiToolBarView
    \brief View class for DuiToolBar widget.

    \ingroup views

    \section DuiToolBarViewOverview Overview
        DuiToolBarView is used to visualize actions placed into the DuiToolBar.

        The outlook of toolbar can be changed using the styling attributes defined
        in DuiToolbarStyle and DuiWidgetStyle.

    \section DuiToolBarInteractions Interactions
        - The toolbar has currently no interactions by itself: the interactions are
          determined by the UI controls that are placed inside the toolbar.
        - The toolbar can be affected by possible interactions of going to and from the
          full screen mode; this would set the visibility of the toolbar on and off.
        - Setting the visibility of the toolbar on/off should be accompanied by a
          (sliding) transition, hiding and revealing the toolbar.

    \section DuiToolBarOpenIssues Open issues
        - Sliding transitions for hiding and revealing the toolbar are not supported yet.

    \sa DuiToolBar DuiToolbarStyle
*/

class DUI_EXPORT DuiToolBarView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiWidgetModel, DuiToolbarStyle)

public:

    /*!
        \brief Constructs toolbar view.
        \param Pointer to the controller.
     */
    DuiToolBarView(DuiToolBar *controller);

    /*!
        \brief Destructs the view.
     */
    virtual ~DuiToolBarView();

protected:
    /*!
        \brief protected constructor
        \param dd Shared private class
        \param controller The controller associated with the view.
    */
    DuiToolBarView(DuiToolBarViewPrivate &dd, DuiToolBar *controller);
    DuiToolBarViewPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(DuiToolBarView)
    Q_DECLARE_PRIVATE(DuiToolBarView)

#ifdef UNIT_TEST
    friend class Ut_DuiToolBarView;
#endif
};

#endif
