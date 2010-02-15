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

#ifndef DUIOBJECTMENU_H
#define DUIOBJECTMENU_H

#include <duiscenewindow.h>
#include <duiobjectmenumodel.h>

class QGraphicsSceneContextMenuEvent;

//! \internal

/*!
    \class DuiObjectMenu
    \brief DuiObjectMenu is a context-menu like widget, which shows object menu actions from any widget.

    \ingroup widgets

    This class is used by DuiWidget and will be launched automatically if the
    widget gets a context menu event and the widget has some object menu
    actions associated to it. To add context menu actions to a widget see the
    code snippet below.
    \code
        // Create an action with title "Edit" and parent it to widget
        // so the action is freed when the widget gets destroyed.
        DuiAction* action = new DuiAction("Edit", widget);

        // Associate this action to object menu.
        action->setLocation(DuiAction::ObjectMenu);

        // And add the action to the widget.
        widget->addAction(action);
    \endcode

    This class can also be used manually, but normally it's enough to just add
    actions to a widget and let the system handle the menu.

    \section DuiObjectMenuOverview Overview
        Object Menu is a popup menu of commands attached to an element that
        contains functions related to it. It can be seen to be quite close to
        the idea of a context-sensitive menu ("right click menu").

        Object menu is opened by long tapping the element. There is a visual
        transition for opening the menu, starting after a certain time period.
        When the menu is activated, the background is dimmed (including Home
        and Back buttons).

    \section DuiObjectMenuUsageGuidlines Usage guidlines
        \li Object menu is a good place for shortcuts: for things that would
            take several clicks and actions to otherwise access from the view
            the user launched the menu from. If an action would take two short
            clicks in the views vs. a long click to open the menu and a click
            in the menu, it doesn't really save time and effort from the users
            anymore.

        \li Object menu promotes certain device features. If you need to make a
            choice between several features, choose features that are to be
            promoted for the users.

        \li Object menu must not be the only place to find a function. Not all
            users will find the object menu.

        \li Although the object menu is scalable, aim to keep the amount of
            commands low (max 6-8) so that panning would not be required. New
            software plug-ins can add functionality to the object menu.

        \li The object menu should not duplicate the primary action (which can
            be accessed by short tapping the element).

        \li For content items, it is mandatory to include "See related" and
            "Mark as favorite" in the Object Menu. "Delete" and "Details" are
            also highly recommended.

        \li Since object menu is a hidden feature, aim to have consistency in
            the menus: aim to have similar functions always for a similar
            content item. The users should have a pretty good guess of whether
            a certain action is in the object menu before they open the menu.

        \li If there is a well known icon for a given function (like search or
            asso-browsing), show the icon also in the object menu for the given
            function.

        \li Be careful in placing commands that are not clear to the users: A
            function like "Call" is very vague: Which number does it call to?
            Which service it will use to make the call?

        \li Selecting commands from the object menu by default should launch
            the item in a subview of the current view, not as a new task.
            (Exceptions can be made in certain cases.)

    \section DuiObjectMenuVariants Variants
        \li \link DuiObjectMenuView Generic view \endlink

    \section DuiObjectMenuWidgetOpenIssues Open issues


    \sa DuiObjectMenuModel DuiObjectMenuStyle
 */

class DuiObjectMenu : public DuiSceneWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(DuiObjectMenu)
    DUI_CONTROLLER(DuiObjectMenu)

public:
    /*!
        \brief Constructs an object menu.

        Object menu is always associated to exactly one widget. The menu shows
        automatically all object menu actions from the \a target widget.
     */
    explicit DuiObjectMenu(DuiWidget *target);

    /*!
        \brief Destructs an object menu.
     */
    virtual ~DuiObjectMenu();

protected:
    //! \reimp
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //! \reimp_end

};
//! \internal_end

#endif
