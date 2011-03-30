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

#ifndef MAPPLICATIONWINDOW_H
#define MAPPLICATIONWINDOW_H

#include <mtheme.h>
#include "mwindow.h"
#include "mnamespace.h"
#include "mexport.h"

class QActionEvent;
class MApplicationWindowPrivate;
class MEscapeButtonPanel;
class MScene;

class MApplicationPage;
class MApplicationPagePrivate;

class MOnDisplayChangeEvent;

/*!
  \class MApplicationWindow
  \brief The MApplicationWindow class provides a top-level application window.

  The application window provides a framework for building an user interface. The window
  provides the navigation bar common to all applications, which includes the home and close
  buttons, a toolbar (in landscape orientation) as well as the application menu.

  A minimum application that shows one window would look like this:

  \code
  #include <MApplication>
  #include <MApplicationWindow>

  int main(int argc, char *argv[]){
      MApplication application(argc, argv);
      MApplicationWindow window;
      window.show();
      return application.exec();
   }
   \endcode

   To insert content into the window, a MApplicationPage has to be created and shown.

   In contrast to MWindow, MApplicationWindow already contains a QGraphicsScene and MSceneManager,
   so that orientation changes and in-scene windows such as MDialog and MApplicationPage windows are
   automatically managed.

   Also MApplicationWindows, unlike plain MWindows, have rounded corners enabled by default.
   Check MWindow::roundedCornersEnabled property for more information.

   \section MApplicationWindowFullScreenMode Full screen mode

   When an application window is in full screen mode (see QWidget::showFullScreen())
   it loses its status bar. To have the status bar again you have to go back to normal mode
   (see QWidget::Normal()).

   Please note that the full screen mode of an application window is orthogonal to the
   presence of the navigation bar and navigation controls (home button and escape button).
   The presence of those is set via MApplicationPage::setComponentsDisplayMode().
  */
class M_CORE_EXPORT MApplicationWindow : public MWindow
{
    Q_OBJECT
    Q_PROPERTY(MTheme::ViewType toolbarViewType READ toolbarViewType WRITE setToolbarViewType)
    Q_PROPERTY(qreal navigationBarOpacity READ navigationBarOpacity WRITE setNavigationBarOpacity)

public:
    /*!
     Creates an application window. A scene and scene manager are created automatically.
     */
    explicit MApplicationWindow(QWidget *parent = 0);

    /*!
     Creates an application window as a view for the given \a scene. A scene manager is created for the scene.
     */
    explicit MApplicationWindow(MScene *scene, QWidget *parent = 0);

    /*!
     Destroys the main window.
     */
    virtual ~MApplicationWindow();

    /*!
     Returns a pointer to the application page that is currently being displayed.
     /sa pageChanged()
     */
    MApplicationPage *currentPage() const;

    /**
     * Returns the iconID of the icon of the window.
     */
    QString windowIconID() const;

    /**
     * Returns the view type of the toolbar for the window.
     * \sa setToolbarViewType()
     */
    MTheme::ViewType toolbarViewType() const;

    /*!
     * Returns true if the menu is opened otherwise false.
     */
    bool isMenuOpen() const;

#ifdef Q_WS_X11
    /*!
     * Returns the window id of the parent task for windows that are chained.
     * \sa isChained()
     */
    WId chainParentWinId() const;

    /*!
     * Returns true if the window is chained otherwise false.
     */
    bool isChained() const;
#endif

    /*!
     * This function updates the chain task data.
     *
     * Do not use this function unless you really know
     * what you are doing !
     *
     * It is used to get the displaying of window animations
     * and the displaying of chained tasks in the switcher
     * view right.
     */
    void updateChainTaskData();

    /*!
      Returns the style name of the application window.
     */
    QString styleName() const;

    /*!
      Returns current opacity of the navigation bar.

      \sa QGraphicsItem::opacity()
    */
    qreal navigationBarOpacity() const;

Q_SIGNALS:
    /*!
     This signal is emitted after \page has become shown.
     */
    void pageChanged(MApplicationPage *page);

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
     * \param view type of the toolbar; possible values are MToolBar::tabType, MToolBar::defaultType
     * \sa MToolBar
     */
    void setToolbarViewType(const MTheme::ViewType& viewType);

    /*!
      Set the style name to \a name.

      This function sets the style name property of the application window to \a name
      and reloads application window style. The style name is used to select a style with
      a matching name from the theme.

      \sa styleName()
    */
    void setStyleName(const QString &name);

    /*!
      Sets the opacity of the navigation bar.

      \sa QGraphicsItem::setOpacity()
    */
    void setNavigationBarOpacity(qreal opacity);

protected:
    //! \internal
    explicit MApplicationWindow(MApplicationWindowPrivate &dd, QWidget *parent = 0);
    explicit MApplicationWindow(MApplicationWindowPrivate &dd, MScene *scene = 0, QWidget *parent = 0);
    //! \internal_end

    //! \reimp
    virtual bool event(QEvent *event);
    virtual void closeEvent(QCloseEvent *event);
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    //! \reimp_end

private:

    void reinit();

    friend class MSceneManagerPrivate;
    friend class MInputWidgetRelocator;
    friend class Ut_MApplicationWindow;
    friend class MComponentCachePrivate;

    Q_DISABLE_COPY(MApplicationWindow)
    Q_DECLARE_PRIVATE(MApplicationWindow)
    Q_PRIVATE_SLOT(d_func(), void _q_actionUpdated(QActionEvent *))
    Q_PRIVATE_SLOT(d_func(), void _q_placeToolBar())
    Q_PRIVATE_SLOT(d_func(), void _q_handlePageModelModifications(const QList<const char *>&))
    Q_PRIVATE_SLOT(d_func(), void _q_menuAppeared())
    Q_PRIVATE_SLOT(d_func(), void _q_menuDisappeared())
#ifdef HAVE_CONTEXTSUBSCRIBER
    Q_PRIVATE_SLOT(d_func(), void _q_updateStatusBarVisibility())
#endif
#ifdef M_OS_MAEMO5
    Q_PRIVATE_SLOT(d_func(), void _q_exitAppView())
#endif //M_OS_MAEMO5
    Q_PRIVATE_SLOT(d_func(), void _q_updatePageEscapeAuto())
    Q_PRIVATE_SLOT(d_func(), void _q_updatePageExposedContentRect())
    Q_PRIVATE_SLOT(d_func(), void _q_navigationBarStateChanged(MSceneWindow::SceneWindowState newState, MSceneWindow::SceneWindowState oldState))
    Q_PRIVATE_SLOT(d_func(), void _q_handleInSwitcherVisibilityChange())
    Q_PRIVATE_SLOT(d_func(), void _q_updateStyle())
    Q_PRIVATE_SLOT(d_func(), void _q_setupNavigationBarCustomContent())
    Q_PRIVATE_SLOT(d_func(), void _q_updateNavigationBarVisibility())
    Q_PRIVATE_SLOT(d_func(), void _q_scheduleNavigationBarVisibilityUpdate())
};

#endif
