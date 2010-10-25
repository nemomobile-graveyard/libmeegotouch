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

#ifndef MAPPLICATIONMENU_H
#define MAPPLICATIONMENU_H

#include <mscenewindow.h>
#include <mapplicationmenumodel.h>

class MApplicationMenuPrivate;

/*!
    \class MApplicationMenu
    \brief MApplicationMenu widget shows a menu containing action commands.

    \ingroup widgets

    \section MApplicationMenuOverview Overview
        Application menu is a menu element that contains \link MAction \endlink and
        \link MWidgetAction \endlink commands which get executed on activation. It consists
        of the title area (icon and optional title) for the menu, displayed in the command
        area of the UI. Application menu can be opened (and closed) by tapping on the menu title area. It
        contains commands and view settings for the view in question. Application menu is
        available in landscape and portrait modes.

        This class is not recommended to be used directly instead action can be added to
        the page using:
        action->setLocation(MAction::ApplicationMenuLocation);

        Here is a simple example that shows how actions are added to the menu.
        \code
        MApplicationMenu* menu = new MApplicationMenu();
        MAction * action = new MAction("Pictures", this);
        action->setLocation(MAction::ApplicationMenuLocation);
        menu->addAction(action);

        //adding a style action
        MAction * styleAction = new MAction("", this);
        styleAction->setLocation(MAction::ApplicationMenuLocation);
        styleAction->setStyleAction(true);
        menu->addAction(styleAction);
        //adding a style action which is checked
        MAction * styleAction = new MAction("", this);
        styleAction->setLocation(MAction::ApplicationMenuLocation);
        styleAction->setStyleAction(true);
        styleAction->setCheckable(true);
        styleAction->setChecked(true);
        menu->addAction(styleAction);
        \endcode

        In some cases, an application may want to assign its own widget to an action. In principle, any widget can
        be assigned to an action, but in the case when an action is added to the application menu (via a call to
        setLocation(MAction::ApplicationMenuLocation) ), only MButtons and MComboBoxes are supported. When
        an application-supplied Widget is set on an action, it MUST do this before adding the action to the page. The
        following example shows this process:
        \code
        MWidgetAction * action = new MWidgetAction("Format", this);
        action->setLocation(MAction::ApplicationMenuLocation);

        MComboBox * formats = new MComboBox(this);
        action->setWidget(formats);

        menu->addAction(action);
        \endcode

    \section MMenuUseGuidelines Usage guidelines
        - Application menu is for commands and actions that are not tied to any of the content items within
          the view, and/or are accessed rarely for the whole lifespan of the use of the application.
        - Each main application menu can (and often should) have different menu contents.
        - Considering the functional roles, the toolbar and the application menu basically share the same role.
          Though the toolbar is optional, all views should have an application menu.
        - The icon for the application menu title should be for the application that is presenting the view in
          question, not for the image type. This is basically for extensibility. For instance, if a video
          file is being shown, there is no point of showing an icon for a video file, but rather showing
          the icon of the application which is providing this footage to the user.
        - Provide a title in the application menu. The title should be a predefined text string describing the
          current view. The title should generally not be a dynamic string, but in some cases this is also allowed
          (e.g. for multiple chat views it would make more sense to put the name of the chat friend in the view
          title instead of having "Chat" in all of them). For single instances, e.g. if the user is in a call, the menu title
          should display "Phone call" (or something similar) and the view contents
          should display the person, the user is talking to. Using predefined strings will also allow us to control
          and handle truncation issues. The title will be displayed in the task switcher for the title of the thumbnail
          for the running task (unless it is desired to specify  a separate title for the task switcher).
        - Use the following guidelines for common application menu commands:
            - "Settings" should be only on main level, unless there are some sub-view specific settings.
            - "Help" command should be made available in all application menus.
            - "Exit task" should be added in application menus so that the user is able to exit from all views
               without having to press Back first. However, it should not be placed on main level, because
               the Close button is available there.
            - the order of these items should be from the top of the menu downwards: Settings, Help, Exit

    \section MApplicationMenuVariants Variants
        \li \link MApplicationMenuView Default application menu view \endlink

    \sa MApplicationMenuModel MApplicationMenuStyle

*/

class M_CORE_EXPORT MApplicationMenu : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MApplicationMenu)

public:

    /*!
        \brief Constructs menu containing no actions with optional \a viewType
    */
    explicit MApplicationMenu(const QString &viewType = QString());

    /*!
        \brief Destructs menu
    */
    virtual ~MApplicationMenu();

protected:

    //! \reimp
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //! \reimp_end

    Q_DECLARE_PRIVATE(MApplicationMenu)

private:
    Q_DISABLE_COPY(MApplicationMenu)

    friend class MApplicationMenuView;
    friend class MMenuObjectView;

#ifdef UNIT_TEST
    friend class Pt_MApplicationMenu;
    friend class Ut_MApplicationMenu;
#endif

};

#endif
