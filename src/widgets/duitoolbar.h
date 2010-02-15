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

#ifndef DUITOOLBAR_H
#define DUITOOLBAR_H

#include "duiwidgetcontroller.h"

class DuiToolBarPrivate;

/*!
    \class DuiToolBar
    \brief DuiToolBar provides a container for buttons and text input field.

    \ingroup widgets

    \section DuiToolBarOverview Overview
        Toolbar is a container for buttons and text input field. It contains actions that are
        relevant to the view but not related to any individual item displayed directly within
        the content area.

        This class is not recommended to be used directly instead action can be added to
        the toolbar in the page using:
        action->setLocation(DuiAction::ToolBarLocation);

        Setting up a new toolbar is quite easy. The following example shows how actions are added
        to a toolbar.
        \code
        DuiToolBar* toolbar = new DuiToolBar();
        DuiAction * iconAction = new DuiAction("Icon-pictures","Pictures", this);
        iconAction->setLocation(DuiAction::ToolBarLocation);
        toolbar->addAction(iconAction);
        DuiTextEdit *entry = new DuiTextEdit();
        entry->setViewType("toolbar");
        DuiWidgetAction *action = new DuiWidgetAction(this);
        action->setLocation(DuiAction::ToolBarLocation);
        action->setWidget(entry);
        toolbar->addAction(action);
        \endcode

    \section DuiToolBarUseGuidelines Usage guidelines
        - The toolbar is functionally in the same role as in the view menu: it has commands that
          are not related specifically to any content item or button displayed within the contents
          of the view.
        - The toolbar should be utilized only if the commands are expected to be used often. The
          toolbar takes away space from the content area. If the commands are not important to the
          view and the main use cases, it is recommendable to not have a toolbar at all, and place
          the commands in the view menu. Similarly, reconsider your design if you would have only
          one or two buttons to the toolbar; commands can be placed to the view menu, or embedded
          to the view.
        - Note that the toolbar is not scalable: do not place commands in the toolbar if the view is
          expected to grow with more functionalities in the future.
        - The commands in the toolbar and in the view menu are not to be duplicated.
        - Since the Direct UI style has no focus for the items in the content view, it is not
          possible to create toolbars that have actions that affect on a single item on screen. For
          instance you shouldn't design a toolbar with "Reply", "Delete", "Move" etc. commands, if
          the assumption is that these commands would apply only to a single item.
        - The toolbar affects the scrollable area and the scroll position indicator in portrait mode.
          The scrollable area ends on top of the toolbar (so that the latest in the list should not
          end up being stuck behind the toolbar)
        - The user is not able to personalize the contents of the toolbar. The application designer
          decides what is shown in toolbar and what in view menu.
        - Application design can also decide whether toolbar is used in both landscape and portrait
          modes. For example, a toolbar can be shown only in landscape mode, and then in portrait
          mode the functionalities are provided somewhere else (for instance in the view menu).
        - The toolbar can be hidden if application designer decides so, for example when using
          full-screen media, virtual keyboard or web browser. Toolbar is hidden and shown together
          with command area.
        - The toolbar remains in a fixed position on screen, on top of contents that are displayed.

    \section DuiToolBarVariants Variants
        \li \link DuiToolBarView Default toolbar view \endlink

    \section DuiToolBarOpenIssues Open issues

    \sa DuiToolbarStyle
*/

class DUI_EXPORT DuiToolBar : public DuiWidgetController
{
    Q_OBJECT

public:

    /*!
        \brief Constructs a toolbar containing no actions with optional \a parent
        and \a viewType
    */
    DuiToolBar(QGraphicsItem *parent = 0, const QString &viewType = "");

    /*!
        \brief Destructs the toolbar object.
    */
    virtual ~DuiToolBar();

protected:

    /*!
        \brief protected constructor
        \param dd Shared private class
        \param model The model. This must not be NULL.
        \param parent Parent widget
    */
    DuiToolBar(DuiToolBarPrivate *dd, DuiWidgetModel *model, QGraphicsItem *parent);

    Q_DECLARE_PRIVATE(DuiToolBar)

private:
    Q_DISABLE_COPY(DuiToolBar)

    friend class DuiToolBarView;

#ifdef UNIT_TEST
    friend class Ut_DuiToolBar;
#endif
};

#endif
