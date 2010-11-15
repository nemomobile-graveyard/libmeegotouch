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

#ifndef MTOOLBAR_H
#define MTOOLBAR_H

#include "mwidgetcontroller.h"

class MToolBarPrivate;

/*!
    \class MToolBar
    \brief MToolBar provides a container for buttons and text input field.

    \ingroup widgets

    \section MToolBarOverview Overview
        Toolbar acts as a container for actions. Depending upon the view attached, it either acts as
        a tab bar or is a container for buttons and text input field. Tab bar is a container for mutually
        exclusive latch-down type of buttons. It contains navigational and sorting actions that are
        relevant to the view but not related to any individual item displayed directly within the
        content area.

        This class is not recommended to be used directly instead action can be added to
        the toolbar in the page using:
        action->setLocation(MAction::ToolBarLocation);

        Setting up a new toolbar is quite easy. The following example shows how actions are added
        to a toolbar.
        \code
        MToolBar* toolbar = new MToolBar();
        MAction * iconAction = new MAction("icon-l-gallery","Pictures", this);
        iconAction->setLocation(MAction::ToolBarLocation);
        toolbar->addAction(iconAction);
        MTextEdit *entry = new MTextEdit();
        entry->setViewType("toolbar");
        MWidgetAction *action = new MWidgetAction(this);
        action->setLocation(MAction::ToolBarLocation);
        action->setWidget(entry);
        toolbar->addAction(action);
        \endcode

        In case it is desired to add actions to tab bar view, it is necessary to set the view of the
        the toolbar to be of tab type. Here is how the view of the toolbar can be changed to tab bar.
        \code
        toolbar->setViewType(MToolBar::tabType);
        \endcode

        Following demonstates how to add a tab action to the toolbar:
        \code
        MToolBar* toolbar = new MToolBar();
        MAction *action = new MAction("icon-l-gallery", NULL, this);
        action->setLocation(MAction::ToolbarLocation);
        action->setCheckable(true);
        action->setChecked(true);
        connect(action, SIGNAL(triggered()), this, SLOT(showPicture()));
        addAction(action);
        \endcode

    \section MToolBarUseGuidelines Usage guidelines
        - The toolbar is functionally in the same role as in the application menu: it has commands that
          are not related specifically to any content item or button displayed within the contents
          of the view.
        - The toolbar should be utilized only if the commands are expected to be used often. The
          toolbar takes away space from the content area. If the commands are not important to the
          view and the main use cases, it is recommendable to not have a toolbar at all, and place
          the commands in the application menu. Similarly, reconsider your design if you would have only
          one or two buttons to the toolbar; commands can be placed to the application menu, or embedded
          to the page.
        - Note that the toolbar is not scalable: do not place commands in the toolbar if the view is
          expected to grow with more functionalities in the future.
        - The commands in the toolbar and in the application menu are not to be duplicated.
        - Since the Direct UI style has no focus for the items in the content view, it is not
          possible to create toolbars that have actions that affect on a single item on screen. For
          instance you shouldn't design a toolbar with "Reply", "Delete", "Move" etc. commands, if
          the assumption is that these commands would apply only to a single item.
        - The toolbar affects the scrollable area and the scroll position indicator in portrait mode.
          The scrollable area ends on top of the toolbar (so that the latest in the list should not
          end up being stuck behind the toolbar)
        - The user is not able to personalize the contents of the toolbar. The application designer
          decides what is shown in the toolbar and what in the application menu.
        - Application design can also decide whether toolbar is used in both landscape and portrait
          modes. For example, a toolbar can be shown only in landscape mode, and then in portrait
          mode the functionalities are provided somewhere else (for instance in the application menu).
        - The toolbar can be hidden if application designer decides so, for example when using
          full-screen media, virtual keyboard or web browser. Toolbar is hidden and shown together
          with command area.
        - The toolbar remains in a fixed position on screen, on top of contents that are displayed.

    \section MToolBarVariants Variants
        \li \link MToolBarView Default toolbar view. \endlink View is used
            to visualize buttons and text input field.
        \li \link MToolbarTabView tab view. \endlink View is used to
            visualize mutually exclusive latch-down type of buttons
    \section MToolBarOpenIssues Open issues

    \sa MToolbarStyle
*/

class M_CORE_EXPORT MToolBar : public MWidgetController
{
    Q_OBJECT

public:
    /*!
        \brief Variable that defines id for tab variant of the toolbar.
    */
    static const MTheme::ViewType tabType;

    /*!
        \brief Constructs a toolbar containing no actions with optional \a parent
        and \a viewType
    */
    MToolBar(QGraphicsItem *parent = 0, const QString &viewType = "");

    /*!
        \brief Destructs the toolbar object.
    */
    virtual ~MToolBar();

protected:

    /*!
        \brief protected constructor
        \param dd Shared private class
        \param model The model. This must not be NULL.
        \param parent Parent widget
    */
    MToolBar(MToolBarPrivate *dd, MWidgetModel *model, QGraphicsItem *parent);

    //! \reimp
    virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);
    //! \reimp_end

    Q_DECLARE_PRIVATE(MToolBar)

private:
    Q_DISABLE_COPY(MToolBar)

    friend class MToolBarView;

#ifdef UNIT_TEST
    friend class Ut_MToolBar;
    friend class Ut_MToolBarView;
#endif
};

#endif
