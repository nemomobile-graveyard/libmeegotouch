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

class M_CORE_EXPORT MNavigationBar : public MSceneWindow
{
    Q_OBJECT
    M_CONTROLLER(MNavigationBar)

#ifdef UNIT_TEST
    friend class Ut_MNavigationBar;
#endif // UNIT_TEST

    /*!
        \property MNavigationBar::escapeButtonMode
        \brief This property holds the mode of the escape button.

        Set it with setEscapeButtonMode() and get its current value with escapeButtonMode().

        \sa MNavigationBarModel::EscapeButtonModeEnum, escapeButtonClicked().
        */
    Q_PROPERTY(MNavigationBarModel::EscapeButtonModeEnum escapeButtonMode READ escapeButtonMode WRITE setEscapeButtonMode)

public:
    /*!
     * \brief Default constructor. Creates a navigation bar.
     */
    MNavigationBar();

    MNavigationBar(QGraphicsItem *parent);

    /*!
      \brief Destroys the navigation bar.
     */
    virtual ~MNavigationBar();

    /**
     * Returns the iconID of the icon of the application menu button.
     */
    QString viewMenuIconID() const;

    //! \deprecated do not use
    /**
     * Returns if the progress indicator is visible
     */
    bool isProgressIndicatorVisible() const;

    /*!
      \brief Returns true if the arrow icon is visible.
    */
    bool isArrowIconVisible() const;

    /*!
        \brief Returns the current mode of the escape button.
        \sa MNavigationBarModel::EscapeButtonModeEnum
    */
    MNavigationBarModel::EscapeButtonModeEnum escapeButtonMode() const;

    /*!
      \brief Returns true if escape button is enabled.
    */
    bool escapeButtonEnabled() const;

    /*!
        \brief Returns true if escape button is visible.
     */
    bool escapeButtonVisible() const;

    //! \internal
    void setCustomContent(QGraphicsWidget *customContent);
    QGraphicsWidget *customContent();
    //! \internal_end

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
     * Sets the new icon of the application menu button.
     * \param the id of the icon that should be shown.
     */
    void setViewMenuIconID(const QString &id);

    //! \deprecated do not use
    /**
     * Sets the visible of progress indicator
     * \param bool visible
     */
    void setProgressIndicatorVisible(bool visible);

    /**
     * Sets the visible of progress indicator
     */
    void setArrowIconVisible(bool visible);

    /*!
        \brief Sets the mode of the escape button.
        \sa MNavigationBarModel::EscapeButtonModeEnum
    */
    void setEscapeButtonMode(MNavigationBarModel::EscapeButtonModeEnum newMode);

    /*!
        \brief Enables or disables escape button.
    */
    void setEscapeButtonEnabled(bool enabled);

    /*!
        \brief Sets the visibility of escape button.
     */
    void  setEscapeButtonVisible(bool visible);

Q_SIGNALS:
    //! \brief viewmenuButton was clicked
    void viewmenuTriggered();

    /*!
        \brief Signal emitted when the escape button in back mode is clicked.
        \sa MNavigationBar::escapeButtonMode
    */
    void backButtonClicked();

    /*!
        \brief Signal emitted when the escape button in close mode is clicked.
        \sa MNavigationBar::escapeButtonMode
    */
    void closeButtonClicked();

private:
    Q_DISABLE_COPY(MNavigationBar)
    Q_DECLARE_PRIVATE(MNavigationBar)
};

#endif
