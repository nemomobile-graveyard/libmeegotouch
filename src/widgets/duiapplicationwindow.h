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

#ifndef DUIAPPLICATIONWINDOW_H
#define DUIAPPLICATIONWINDOW_H

#include <duitheme.h>
#include "duiwindow.h"
#include "duinamespace.h"
#include "duiexport.h"

class QActionEvent;
class DuiApplicationWindowPrivate;
class DuiEscapeButtonPanel;
class DuiScene;

class DuiApplicationPage;
class DuiApplicationPagePrivate;

class DuiOnDisplayChangeEvent;

/*!
  \class DuiApplicationWindow
  \brief The DuiApplicationWindow class provides a top-level application window.

  The application window provides a framework for building an user interface. The window
  provides the navigation bar common to all applications, which includes the home and close
  buttons, a toolbar (in landscape orientation) as well as the application menu.

  A minimum application that shows one window would look like this:

  \code
  #include <DuiApplication>
  #include <DuiApplicationWindow>

  int main(int argc, char *argv[]){
      DuiApplication application(argc, argv);
      DuiApplicationWindow window;
      window.show();
      return application.exec();
   }
   \endcode

   To insert content into the window, a DuiApplicationPage has to be created and shown.

   In contrast to DuiWindow, DuiApplicationWindow already contains a QGraphicsScene and DuiSceneManager,
   so that orientation changes and in-scene windows such as DuiDialog and DuiApplicationPage windows are
   automatically managed.
  */
class DUI_EXPORT DuiApplicationWindow : public DuiWindow
{
    Q_OBJECT
    Q_PROPERTY(DuiTheme::ViewType toolbarViewType READ toolbarViewType WRITE setToolbarViewType)

public:
    /*!
     Creates an application window. A scene and scene manager are created automatically.
     */
    explicit DuiApplicationWindow(QWidget *parent = 0);

    /*!
     Creates an application window as a view for the given \a scene. A scene manager is created for the scene.
     */
    explicit DuiApplicationWindow(DuiScene *scene, QWidget *parent = 0);

    /*!
     Destroys the main window.
     */
    virtual ~DuiApplicationWindow();

    /*!
     Returns a pointer to the currently displayed application page.
     /sa pageChanged()
     */
    DuiApplicationPage *currentPage() const;

    /**
     * Returns the iconID of the icon of the window.
     */
    QString windowIconID() const;

    /**
     * Returns the view type of the toolbar for the window.
     * \sa setToolbarViewType()
     */
    DuiTheme::ViewType toolbarViewType() const;

    /*!
     * Returns true if the menu is opened otherwise false.
     */
    bool isMenuOpen() const;

Q_SIGNALS:
    /*!
     This signal is emitted after \page has become shown.
     */
    void pageChanged(DuiApplicationPage *page);

public Q_SLOTS:

    /*!
     * Opens the application menu of the window, provided that there
     * are some actions on the current application page.
     */
    void openMenu();

    /*!
     * Closes the application menu of the window.
     */
    void closeMenu();

    /**
     * Sets the new icon of the window.
     */
    void setWindowIconID(const QString &windowIconID);

    /**
     * Sets the view type of the toolbar for the window.
     * \param view type of the toolbar; possible values are DuiToolBar::tabType, DuiToolBar::defaultType
     * \sa DuiToolBar
     */
    void setToolbarViewType(const DuiTheme::ViewType& viewType);

protected:
    //! \internal
    explicit DuiApplicationWindow(DuiApplicationWindowPrivate &dd, QWidget *parent = 0);
    explicit DuiApplicationWindow(DuiApplicationWindowPrivate &dd, DuiScene *scene = 0, QWidget *parent = 0);
    //! \internal_end

    //! \reimp
    virtual bool event(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);

    // TODO: Remove that now useless method override after API freeze period
    virtual void onDisplayChangeEvent(DuiOnDisplayChangeEvent *event);
    //! \reimp_end

private:
    friend class DuiSceneManagerPrivate;

    Q_DISABLE_COPY(DuiApplicationWindow)
    Q_DECLARE_PRIVATE(DuiApplicationWindow)
    Q_PRIVATE_SLOT(d_func(), void _q_pageTitleChanged(DuiApplicationPage *, const QString &))
    Q_PRIVATE_SLOT(d_func(), void _q_actionUpdated(QActionEvent *))
    Q_PRIVATE_SLOT(d_func(), void _q_placeToolBar(Dui::Orientation))
    Q_PRIVATE_SLOT(d_func(), void _q_handlePageModelModifications(const QList<const char *>&))
    Q_PRIVATE_SLOT(d_func(), void _q_menuAppeared())
    Q_PRIVATE_SLOT(d_func(), void _q_menuDisappeared())
#ifdef HAVE_N900
    Q_PRIVATE_SLOT(d_func(), void _q_exitAppView())
#endif
    Q_PRIVATE_SLOT(d_func(), void _q_updatePageEscapeAuto())
};

#endif
