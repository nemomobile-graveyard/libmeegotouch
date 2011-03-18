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

#ifndef MWINDOW_H
#define MWINDOW_H

#include "mexport.h"
#include "mnamespace.h"

#include <QGraphicsView>

class MWindowPrivate;
class MWidget;
class MOrientationChangeParameters;
class MScene;
class MSceneManager;
class MOnDisplayChangeEvent;

/*!
  \class MWindow
  \brief MWindow is the base class for all MeeGo Touch UI windows

  An MWindow is completely empty by default. For developing an application
  conforming to the MeeGo Touch UI style, use MApplicationWindow instead.

  MWindow is a QGraphicsView widget that visualizes the contents
  of the application scene. The key differences compared to the standard
  QGraphicsView is:

  - The coordinate system of a MWindow starts with (0,0) in the top-left corner
    and does typically not change during an application's execution.
  - MWindows have an OpenGL viewport by default, which causes QPainter commands
    for the window to use one of Qt's OpenGL backends.
  - Fullscreen updates are enabled by default for the window, as OpenGL partial
    updates are not supported in Qt.
  - New concept of window orientation. MeeGo Touch applications change orientation through
    a scene graph transformation. The window widget coordinates stay the same
    after an orientation change while the root element of the scene graph can have
    an arbitary transformation angle. See \ref rotation.
  - New concept of scene management. MeeGo Touch UI supports in-scene variants of traditional
    UI components such as dialogs, popups and menus. MSceneManager acts like an
    in-application window manager to orchestrate the various components.
  - New concept of display visibility. MeeGo Touch UI applications typically operate in
    a fully composited environment, meaning that a shown window still doesn't
    guarantee that it is visible. Display visibility tells whether the user is
    able to see the window.

  Example: Showing scene windows on MWindow.

  \code
      MApplication app;
      MWindow window;
      MSceneWindow *sceneWindow = new MMessageBox("Hello World!");

      window.show();

      sceneWindow->appear(&window);

      app.exec();
  \endcode
*/
class M_CORE_EXPORT MWindow : public QGraphicsView
{

    Q_OBJECT
    /*!
     \property orientationAngleLocked
     Is the orientation angle locked
     */
    Q_PROPERTY(bool orientationAngleLocked READ isOrientationAngleLocked WRITE setOrientationAngleLocked)
    /*!
     \property orientationLocked
     Is the orientation locked
     */
    Q_PROPERTY(bool orientationLocked READ isOrientationLocked WRITE setOrientationLocked)

    /*!
     \property globalAlpha
     The global alpha of the hardware graphics overlay for use by this window.
    */
    Q_PROPERTY(qreal globalAlpha READ globalAlpha WRITE setGlobalAlpha)

    /*!
     \property videoGlobalAlpha
     The global alpha of the hardware video overlay for use by this window.
    */
    Q_PROPERTY(qreal videoGlobalAlpha READ videoGlobalAlpha WRITE setVideoGlobalAlpha)

    /*!
     \property notificationPreviewsVisible
     Are notification preview banners shown on top of the window.
    */
    Q_PROPERTY(bool notificationPreviewsVisible READ notificationPreviewsVisible WRITE setNotificationPreviewsVisible)

    /*!
     \property MWindow::roundedCornersEnabled
     \brief This property defines whether the window should have rounded corners.

     By default this property is false.

     Get its current value with isRoundedCornersEnabled() and set it with
     setRoundedCornersEnabled();

     \note Setting it to true will incur the creation of a scene manager
           and scene if the window doesn't have them yet.
    */
    Q_PROPERTY(bool roundedCornersEnabled READ isRoundedCornersEnabled WRITE setRoundedCornersEnabled)

public:
    /*!
     \brief Creates a MWindow without a scene manager.

     \a parent is passed to QGraphicsView's constructor.

     By default MWindow doesn't create a scene manager. It can be assigned
     later by calling setSceneManager(). Also, a default scene manager
     will be automatically created as needed unless explicitly set whenever
     any scene management is required (adding a managed class like MDialog
     to the application).

     \sa setSceneManager(), sceneManager()
     */
    explicit MWindow(QWidget *parent = 0);

    /*!
     \brief Creates a MWindow with a provided \a scenemanager.

     \a parent is passed to QGraphicsView's constructor.

     MWindow will take ownership of the given scene manager.
     */
    explicit MWindow(MSceneManager *sceneManager,
                       QWidget *parent = 0);

    /*!
     \brief Destructs the MWindow object.

     If a scene manager is associated with the window, it is destroyed as well.
     */
    virtual ~MWindow();

    /*!
     \brief Sets the translucency of the window.

     Background translucency is disabled by default.

     Having a translucent background means that any window behind it will
     show through the parts not otherwise filled by the application, for example
     by widgets.

     Calling this method is costly when invoking it after showing the window as it
     involves recreating the window's viewport.
     This method usually should be called only once, before showing the window.

     Platform notes:
      - Device: This feature requires that a compositing manager is running
        and an EGL surface with alpha channels
      - Desktop/Scratchbox: MesaGL does not support ARGB GLX visuals. As a
        workaround, translucency also works in software mode.
     */
    void setTranslucentBackground(bool enabled);

    /*!
    * \brief Sets the global alpha of the hardware graphics overlay for use
    * by this window.
    *
    * Global alpha is used when you want to blend the contents of this window,
    * which is rendering to the graphics overlay with the video overlay.
    * This can be used to draw translucent controls on top of video,
    * for example.
    *
    * By default, each MWindow does not enable the global alpha. When this
    * window is destroyed or hidden, the system will revert the global alpha
    * back to its previous state.
    *
    *  Platform notes:
    *   - This is a Harmattan-specific implementation and will require
    *     interaction with the Harmattan compositing window manager
    *     (MCompositor)
    *
    * \param level The valid range of global alpha can be 1.0 (completely opaque) to 0.0 (completely transparent).
    */
    void setGlobalAlpha(qreal level);

   /*!
     \brief Returns the global alpha of the hardware graphics overlay for use
     by this window.
     */
    qreal globalAlpha();

    /*!
     * \brief Sets the alpha of the hardware video overlay for use
     * by this window.
     *
     * Video alpha is the is used to control the dimming of the video overlay
     * where hardware accelerated video is rendering.
     *
     * By default, each MWindow does not enable the video alpha. When this
     * window is destroyed or hidden, the system will revert the video alpha
     * back to its previous state.
     *
     *  Platform notes:
     *   - This is a Harmattan-specific implementation and will require
     *     interaction with the Harmattan compositing window manager
     *     (MCompositor)
     *
     * \param level The valid range of video alpha can be 1.0 (completely opaque) to 0.0 (completely transparent)
     */
    void setVideoGlobalAlpha(qreal level);

    /*!
     * \brief Returns the alpha of the hardware video overlay for use by this window.
     */
    qreal videoGlobalAlpha();

    /*!
     \brief Returns the window's MScene.
     */
    MScene *scene();

    /*!
     \brief Assigns a scene manager to the window.

     The existing scene manager (if any) will be deleted and replaced by
     \a sceneManager. The new \a sceneManager can be 0.

     MWindow will take ownership of the given scene manager.

     \note  Scene manager takes over orientation angle management of a window.
            If window orientation angle was changed manually before creating
            a scene manager, the scene manager will override this setting.

     */
    void setSceneManager(MSceneManager *sceneManager);

    /*!
     \brief Returns the scene manager of the window, or 0 if not set.

     The scene manager is responsible for positioning, animating and
     generally managing MSceneWindows such as MDialog and MApplicationPage
     inside the window's scene.
     */
    MSceneManager *sceneManager() const;

    /*!
     \brief Returns the scene manager of the window. A scene manager will
     be automatically created if none was previously set.

     The scene manager is responsible for positioning, animating and
     generally managing MSceneWindows such as MDialog and MApplicationPage
     inside the window's scene.

     \note  Scene manager takes over orientation angle management of a window.
            If window orientation angle was changed manually before creating
            a scene manager, the scene manager will override this setting.

     \sa setSceneManager()
     */
    MSceneManager *sceneManager();

    /*!
     \brief Returns the current orientation of the window's scene.
     */
    M::Orientation orientation() const;

    /*!
     \brief Returns the current orientation angle of the window's scene.
     */
    M::OrientationAngle orientationAngle() const;

    /*!
     \brief Returns the visible size of the window (being also the size of its scene)
     for the given \a orientation.
     */
    QSize visibleSceneSize(M::Orientation) const;

    /*!
     Returns the visible size of the window (being also the size of its scene).
     */
    QSize visibleSceneSize() const;

    /*!
     Returns true if the window orientation angle is not allowed to change
     when device orientation changes.

     The default value is false.

     \sa setOrientationAngleLocked()
     */
    bool isOrientationAngleLocked() const;

    /*!
     Controls whether the window orientation angle follows the device orientation.

     If you require your window to be displayed only in a certain orientation angle,
     set the desired angle using setOrientationAngle() and set this property to true.

     Note: Locking the orientation is discouraged by the MeeGo Touch UI application style guidelines.

     Example of locking the window orientation angle:

     \code
        MApplication app;
        MWindow window;

        // Forces an orientation angle.
        window.setOrientationAngle(M::Angle270);
        window.setOrientationAngleLocked(true);

        window.show();

        return app.exec();
     \endcode

     \sa lockOrientationAngle(), unlockOrientationAngle()
     */
    void setOrientationAngleLocked(bool locked);

    /*!
     Returns true if the window orientation is not allowed to change
     when device orientation changes.

     The default value is false.

     \sa setOrientationLocked()
     */
    bool isOrientationLocked() const;

     /*!
     Controls whether the window orientation follows the device orientation.

     If you require your window to be displayed only in a certain orientation,
     set the desired angle using setOrientationAngle() and set this property to true.

     Note: Locking the orientation is discouraged by the MeeGo TouchUI application style
     guidelines.

     \note Setting this property to true locks orientation changes,
           but allows automatic adjustments of the angle for the given orientation.
           This means that a window locked in portrait mode will adjust between
           M::Angle90 and M::Angle270 to keep "top edge" always on top and avoid
           upside-down situations. In order to lock to a single specific orientation
           angle, use setOrientationAngleLocked().

     Example of locking the window orientation:

     \code
        MApplication app;
        MWindow window;

        // Forces the orientation to be portrait
        window.setPortraitOrientation();
        window.setOrientationLocked(true);

        window.show();

        return app.exec();
     \endcode

     \sa lockOrientationAngle(), unlockOrientationAngle()
     */
    void setOrientationLocked(bool locked);

    /*! Returns true if user can see the window.
     *  This is different from QWidget::isVisible(), which is always
     *  explicitly set in the application. QWidget::isVisible() doesn't
     *  know if the window is e.g. covered by an opaque window and cannot be
     *  seen.
     *
     *  \note Translucent windows (windows that have an alpha channel) are not
     *  considered to obscure the windows behind them, even if all their pixels
     *  happen to have alpha 1.0.
     *
     * \sa displayEntered(), displayExited()
     */
    bool isOnDisplay() const;

    /*! Returns true if the window is currently minimized to the switcher.
     *  It doesn't matter whether the window is visible or not in the
     *  switcher.
     */
    bool isInSwitcher() const;

    /*!
     * Allows to force close instead of hide during lazy shutdown.
     */
    void setCloseOnLazyShutdown(bool enable);

    //! Returns true if window closes on lazy shutdown.
    bool closeOnLazyShutdown() const;

    /*!
      Controls whether notification preview banners are shown on top of the window.

      \note System notifications (e.g. battery low) are still shown regardless of the property.
     */
    void setNotificationPreviewsVisible(bool visible);

    /*!
     * Returns true if application notification preview banners are shown over
     * this window
     *
     * The value is true by default.
     */
    bool notificationPreviewsVisible() const;

    /*!
      \brief Returns whether the window has/draws rounded corners.
      \sa setRoundedCornersEnabled()
     */
    bool isRoundedCornersEnabled() const;

    /*!
      \brief Sets whether the window should have/draw rounded corners.
      \sa isRoundedCornersEnabled()
     */
    void setRoundedCornersEnabled(bool enabled);


public Q_SLOTS:
    /*!
     Sets the orientation \a angle of the window.

     If the window is hidden or it hasn't been shown yet, the orientation angle change
     is immediate (i.e. without an rotation animation), otherwise there may be an
     animated transition.
     */
    void setOrientationAngle(M::OrientationAngle angle);

    /*!
     Sets the orientation of the window to landscape.

     This is a convenience function that sets the current orientation angle to 0 if
     it is not already in a landscape orientation (either 0 or 180 degrees angle).

     If the window is hidden or it hasn't been shown yet, the orientation change
     is immediate (i.e. without an rotation animation), otherwise there may be an
     animated transition.
     */
    void setLandscapeOrientation();

    /*!
     Sets the orientation of the window to portrait.

     This is a convenience function that sets the current orientation angle to 270 if
     it is not already in a portrait orientation (either 90 or 270 degrees angle).

     If the window is hidden or it hasn't been shown yet, the orientation change
     is immediate (i.e. without an rotation animation), otherwise there may be an
     animated transition.
     */
    void setPortraitOrientation();

    /*!
     Locks window's orientation angle changes. Equal to calling setOrientationAngleLocked(true).

     \sa setOrientationAngleLocked(), unlockOrientationAngle()
     */
    void lockOrientationAngle();

    /*!
     Unlocks window's orientation angle changes. Equal to calling setOrientationAngleLocked(false).

     \sa setOrientationAngleLocked(), lockOrientationAngle()
     */
    void unlockOrientationAngle();

    /*!
     Locks window's orientation changes. Equal to calling setOrientationLocked(true).

     \sa setOrientationLocked(), unlockOrientation()
     */
    void lockOrientation();

    /*!
      Unlocks window's orientation changes. Equal to calling setOrientationLocked(false).

     \sa setOrientationLocked(), lockOrientation()
     */
    void unlockOrientation();

    /*!
     Allows notification preview banners to appear over this window. Equal to calling setNotificationPreviewsVisible(true).

     \sa setNotificationPreviewsVisible(), notificationPreviewsVisible()
     */
    void enableNotificationPreviews();

    /*!
     Prevents notification preview banners from appearing over this window. Equal to calling setNotificationPreviewsVisible(false).

     \sa setNotificationPreviewsVisible(), notificationPreviewsVisible()
     */
    void disableNotificationPreviews();

Q_SIGNALS:
    /*!
     Emitted after the scene geometry has changed during rotation.

     This is for widgets that need to react when the orientation is about to change,
     and is emitted after the scene geometry has changed and the rotation animation
     is about to start.

     \note This signal is emitted at start of the rotation animation.
     \sa orientationChanged()
     */
    void orientationAngleChanged(M::OrientationAngle);

    /*!
     Emitted after scene geometry has changed for a rotation.

     This is for widgets that need to react when the orientation is about to change,
     and is emitted after the scene geometry has changed and the rotation animation
     is about to start.

     \note This signal is emitted at start of the rotation animation.
     \sa orientationAngleChanged()
     */
    void orientationChanged(M::Orientation);

    /*!
     This signal is emitted when the rotation animation (if any) has finished.
     It's emitted in tandem with MSceneManager::orientationChangeFinished().

     If window doesn't have the scene manager, this signal is emitted together with
     orientationChanged().

     \note This signal is emitted with every orientation angle change.

     \sa orientationChanged()
     */
    void orientationChangeFinished(M::Orientation);

    /*!
     This signal is emitted when:
     - the window becomes visible
     - is not obscured anymore by an opaque window

     Calling show() will emit displayEntered() when the window becomes
     visible.

     \sa displayExited(), isOnDisplay()
     */
    void displayEntered();

    /*!
     A signal that is emitted when:
     - the window has been obscured by an opaque window approximately for one second
     - the window gets hidden
     - the window gets closed

     Calling close() will immediately emit displayExited().

     \sa displayEntered(), isOnDisplay()
     */
    void displayExited();

    /*!
     This signal is emitted when the window has entered the task switcer.
     */
    void switcherEntered();

    /*!
     This signal is emitted when the window has exited the task switcer.
     */
    void switcherExited();

protected:
    //! \reimp
    bool event(QEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    //! \reimp_end

    /*!
     This event handler allows a window to notify subscribers about
     changes in its presence on the display. enterDisplayEvent() and exitDisplayEvent()
     convenience handlers are called by the default implementation. MOnDisplayChangeEvent
     is sent e.g. if the window gets obscured by another window.
     */
    virtual void onDisplayChangeEvent(MOnDisplayChangeEvent *event);

    /*!
     * A handler that is called just before displayEntered() is emitted.
     */
    virtual void enterDisplayEvent();

    /*!
     * A handler that is called just before displayExited() is emitted.
     */
    virtual void exitDisplayEvent();

    //! \internal
    explicit MWindow(MWindowPrivate &dd, QWidget *parent = 0);
    explicit MWindow(MWindowPrivate &dd, MScene *scene, QWidget *parent = 0);
    explicit MWindow(MWindowPrivate &dd, MSceneManager *sceneManager, QWidget *parent = 0);

    virtual void setVisible(bool visible);

    MWindowPrivate *const d_ptr;
    //! \internal_end

private:
    Q_DISABLE_COPY(MWindow)
    Q_DECLARE_PRIVATE(MWindow)
    Q_PRIVATE_SLOT(d_func(), void _q_onPixmapRequestsFinished())
    Q_PRIVATE_SLOT(d_func(), void _q_enablePaintUpdates())
    Q_PRIVATE_SLOT(d_func(), void _q_exitDisplayStabilized())

    /**
     * Is invoked by MComponentCache in case if an application is
     * prestarted.
     */
    void prestartedInit();

    /*
        For testing purposes.
        Returns a MSceneManager::TransitionMode
    */
    int orientationChangeTransitionMode();

    friend class MApplicationPrivate;
    friend class MSceneManagerPrivate;
    friend class MOrientationTrackerPrivate;
    friend class MComponentCachePrivate; // Calls MWindow::prestartedInit()
#ifdef UNIT_TEST
    // to call orientationAngleChanged()
    friend class Ut_MWindow;
    friend class Ut_MSceneManager;
    friend class Ut_MOrientationTracker;
#endif
};

#endif
