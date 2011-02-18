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

#ifndef MAPPLICATIONMENUVIEW_H
#define MAPPLICATIONMENUVIEW_H

#include "mscenewindowview.h"
#include <mapplicationmenustyle.h>
#include "mapplicationmenumodel.h"
#include <mnamespace.h>

class MApplicationMenu;
class MApplicationMenuViewPrivate;

/*!
    \class MApplicationMenuView
    \brief View class for MApplicationMenu.

    \ingroup views

    \section MApplicationMenuViewOverview Overview
        MApplicationMenuView is used to visualize actions placed into the MApplicationMenu.
        MApplicationMenuView appears once the user clicks on the application menu title area. It is opened on
        top of the current application view. Background of the view is dimmed/blurred when application
        menu is open. The outlook of application menu can be changed using the styling attributes defined in
        MApplicationMenuStyle and MSceneWindowStyle.

        The following action widgets are supported for this particular view:
        - Button, Icon button variant
        - Combobox

        \subsection MApplicationMenuViewCommands Commands
         - There are two main types of commands: view style commands and action commands
         - The view style commands set the style/order/presentation of the contents of the view, while
           still staying inside the same view. Examples of style commands include a setting for list/grid
           style presentation for the current view, or setting abc/recent/favourites sorting order for
           the contents of the view
         - Style commands are grouped and mutually exclusive, so only one can be selected at any time.
         - Action commands can open new views or dialogs, essentially leaving the current view and presenting
           a new UI, in the same task or then in a new task
         - The view can contain only view style commands, only action commands, or both of them
         - The view style and action commands are grouped together and presented separately; the view
           style commands always first (on the top of the menu)
         - In portrait mode, the commands are presented one item per row, vertically from top to bottom
         - In landscape mode, the commands are presented in two items per row, horizontally from left to
           right, then from top to bottom.
         - The user should be able to turn the device orientation after opening the menu, which will redraw
           the menu in the new orientation.
         - Maximum number of action commands is 8, and in presence of style commands, maximum number of
           action commands reduces to 6.

    \section MApplicationMenuInteractions Interactions
        - The view can be closed by tapping anywhere outside the menu (anywhere in the dimmed area)
        - The view can also be closed from the top of screen (from the application menu title, or the top right corner)
        - Selecting an action command from the application menu closes the menu
        - Selecting a view style command closes the application menu and sets the view and its contents in
          the newly selected style.

    \sa MApplicationMenu MApplicationMenuModel MApplicationMenuStyle

*/

class M_VIEWS_EXPORT MApplicationMenuView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MApplicationMenuModel, MApplicationMenuStyle)

protected:
    MApplicationMenuViewPrivate *const d_ptr;

public:
    /*!
        \brief Constructs MApplicationMenuView with a pointer to the controller
     */
    MApplicationMenuView(MApplicationMenu *controller);

    /*!
     * \brief Destructs MApplicationMenuView
     */
    virtual ~MApplicationMenuView();

    //! \reimp
    virtual void drawBackground(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;
    //! \reimp_end

protected:
    //! \reimp
    virtual void applyStyle();
    //! \reimp_end
    MApplicationMenuView(MApplicationMenuViewPrivate &dd, MApplicationMenu *controller);

private:
    Q_DISABLE_COPY(MApplicationMenuView)
    Q_DECLARE_PRIVATE(MApplicationMenuView)

#ifdef UNIT_TEST
    friend class Pt_MApplicationMenu;
    friend class Ut_MApplicationMenu;
    friend class Ut_MApplicationMenuView;
#endif

};


#endif
