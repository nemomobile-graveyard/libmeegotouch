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

#ifndef MOBJECTMENU_H
#define MOBJECTMENU_H

#include <mscenewindow.h>
#include <mobjectmenumodel.h>

class QGraphicsSceneContextMenuEvent;

//! \internal

/*!
    \class MObjectMenu
    \brief MObjectMenu is a context-menu like widget, which shows object menu 
           actions from any widget or from itself.

    \ingroup widgets

    This class can be used with or without a MWidget as a source for the actions. 
    If target widget is not defined the actions are gathered only from the 
    object menu instance itself.
    
    Menu is launched automatically by MWidget when it gets a context menu event 
    and it has some object menu actions associated to it. To add context menu 
    actions to a widget see the code snippet below.
    \code
        // Create an action with title "Edit" and parent it to widget
        // so the action is freed when the widget gets destroyed.
        MAction* action = new MAction("Edit", widget);

        // Associate this action to object menu.
        action->setLocation(MAction::ObjectMenu);

        // And add the action to the widget.
        widget->addAction(action);
    \endcode

    When using this class without a target widget, the actions need to be added 
    manually to the instance of MObjectMenu class. In this case, the menu needs to 
    be shown manually as well.
    \code
        //manually initialized an instance of MObjectMenu
        MObjectMenu* objectMenu = new MObjectMenu(NULL);
        .
        .
        .       
        // Create an action with title "Edit" and parent it to objecMenu
        // so the action is freed when the menu gets destroyed.
        MAction* action = new MAction("Edit", objectMenu);

        // Associate this action to object menu.
        action->setLocation(MAction::ObjectMenu);

        // And add the action to object menu instance
        objectMenu->addAction(action);
        .
        .
        .
        //manually show the object menu when needed
        sceneManager()->appearSceneWindow(objectMenu);
    \endcode
    
    Please notice that in most cases it should be enough to use the automatic 
    object menu launching functionality provided by the MWidget.

    \section MObjectMenuOverview Overview
        Object Menu is a popup menu of commands attached to an element that
        contains functions related to it. It can be seen to be quite close to
        the idea of a context-sensitive menu ("right click menu").

        Usually the object menu is opened by long tapping the element. There is 
        a visual transition for opening the menu, starting after a certain time 
        period. When the menu is activated, the background is dimmed (including 
        Home and Back buttons).

    \section MObjectMenuUsageGuidlines Usage guidlines
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

    \section MObjectMenuVariants Variants
        \li \link MObjectMenuView Generic view \endlink

    \section MObjectMenuWidgetOpenIssues Open issues


    \sa MObjectMenuModel MObjectMenuStyle
 */

class M_CORE_EXPORT MObjectMenu : public MSceneWindow
{
    Q_OBJECT
    Q_DISABLE_COPY(MObjectMenu)
    M_CONTROLLER(MObjectMenu)

    Q_PROPERTY(QPointF cursorPosition READ cursorPosition WRITE setCursorPosition)
    Q_PROPERTY(QString iconId READ iconId WRITE setIconId)
    Q_PROPERTY(QString title READ title WRITE setTitle)
public:
    /*!
        \brief Constructs an object menu.

        Object menu can be associated with a \a target widget. If \a target 
        widget is valid, menu will show automatically all the object menu 
        actions from the specified widget. If \a target is 0, menu includes 
        only the actions that will be added to the MObjectMenu instance itself.
     */
    explicit MObjectMenu(MWidget *target);

    /*!
        \brief Constructs an object menu with a specified actions.
     */
    explicit MObjectMenu(const QList<MAction *>& actionList);

    /*!
        \brief Destructs an object menu.
     */
    virtual ~MObjectMenu();

    QPointF cursorPosition() const;
    void setCursorPosition(const QPointF &pos);

    /*!
        \brief Get the current title string.
     */
    QString title() const;

    /*!
        \brief Set the title string.

        Alternatively, the title can be defined by setting the dynamic property
        named "objectMenuTitle" for the target widget that is given to the
        constructor of this class before constructing this MObjectMenu.
     */
    void setTitle(const QString& title);

    /*!
        \brief Get the current icon id.
    */
    QString iconId() const;

    /*!
      \brief Set the icon id.

        Alternatively, the icon id can be defined by setting the dynamic property
        named "objectMenuIconId" for the target widget that is given to the
        constructor of this class before constructing this MObjectMenu.
     */
    void setIconId(const QString& iconId);


protected:
    //! \reimp
    virtual bool eventFilter(QObject *obj, QEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //! \reimp_end
};
//! \internal_end

#endif
