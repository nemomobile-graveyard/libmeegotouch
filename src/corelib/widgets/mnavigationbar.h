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

#ifndef MNAVIGATIONBAR_H
#define MNAVIGATIONBAR_H

#include <mscenewindow.h>
#include <mnavigationbarmodel.h>

class MNavigationBarPrivate;
class MToolBar;

/*!
 * \class MNavigationBar
 * \brief MNavigationBar implements a navigation bar with viewmenu
 *
 * MNavigationBar doesn't have that many parameters which are read from
 * theme. The theming is done changing the items which lay inside the
 * navigation bar.
 */

class M_EXPORT MNavigationBar : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MNavigationBar)

#ifdef UNIT_TEST
    friend class Ut_MNavigationBar;
#endif // UNIT_TEST

public:
    /*!
     * \brief Default constructor. Creates a navigation bar.
     */
    MNavigationBar();

    /*!
      \brief Destroys the navigation bar.
     */
    virtual ~MNavigationBar();

    /**
     * Returns the iconID of the icon of the view menu button.
     */
    QString viewMenuIconID() const;

    /**
     * Returns if the progress indicator is visible
     */
    bool isProgressIndicatorVisible() const;

    /*!
      \brief Returns true if the arrow icon is visible.
    */
    bool isArrowIconVisible() const;

public Q_SLOTS:
    /** \brief Trigger notification of the user via the navigation bar.
     *
     * This slot asks the NavigationBarView to notify the user, e.g.
     * by flashing the Home button.
     */
    void notifyUser();

    /** \brief Dock a toolbar.
     * \param toolbar The toolbar to dock.
     */
    void dockToolBar(MToolBar *toolbar);

    /** \brief Undock a toolbar.
     */
    void undockToolBar();

    void setViewMenuDescription(const QString &text);

    /**
     * Sets the new icon of the view menu button.
     * \param the id of the icon that should be shown.
     */
    void setViewMenuIconID(const QString &id);

    /**
     * Sets the visible of progress indicator
     * \param bool visible
     */
    void setProgressIndicatorVisible(bool visible);

    /**
     * Sets the visible of progress indicator
     */
    void setArrowIconVisible(bool visible);

Q_SIGNALS:
    //! \brief viewmenuButton was clicked
    void viewmenuTriggered();

private:
    Q_DISABLE_COPY(MNavigationBar)
    Q_DECLARE_PRIVATE(MNavigationBar)
};

#endif
