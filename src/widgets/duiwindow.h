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

#ifndef DUIWINDOW_H
#define DUIWINDOW_H

#include "duiexport.h"
#include "duinamespace.h"

#include <QGraphicsView>

class DuiWindowPrivate;
class DuiWidget;
class DuiOrientationChangeParameters;
class DuiScene;
class DuiSceneManager;
class DuiOnDisplayChangeEvent;

/*!
  \class DuiWindow
  \brief DuiWindow is the base class for all Direct UI application windows

    Example: Showing scene windows on DuiWindow.

    \code
        DuiApplication app;
        DuiWindow window;
        DuiSceneWindow *sceneWindow = new DuiMessageBox("Hello World!");

        window.show();

        sceneWindow->appear(&window);

        app.exec();
    \endcode

*/
class DUI_EXPORT DuiWindow : public QGraphicsView
{

    Q_OBJECT
    /*!
     * \property keepCurrentOrientation
     * \deprecated Since 0.18.7. Use orientationLocked instead.
     * \sa isOrientationLocked(), setOrientationLocked()
     */
    Q_PROPERTY(bool keepCurrentOrientation READ keepCurrentOrientation WRITE setKeepCurrentOrientation)
    Q_PROPERTY(bool orientationAngleLocked READ isOrientationAngleLocked WRITE setOrientationAngleLocked)
    Q_PROPERTY(bool orientationLocked READ isOrientationLocked WRITE setOrientationLocked)

public:
    /*!
     * \brief Creates a DuiWindow without a scene manager.
     *
     * A scene manager can still be assigned later on by calling
     * setSceneManager(). One will also be automatically created
     * if needed by the non-const version of sceneManager().
     *
     * \param parent QWidget defaults to 0
     * \sa setSceneManager(), sceneManager()
     */
    explicit DuiWindow(QWidget *parent = 0);

    /*!
     * \brief Creates a DuiWindow with a provided scene manager.
     * DuiWindow will take ownership over the given scene manager.
     *
     * \param sceneManager Scene manager to be used.
     */
    explicit DuiWindow(DuiSceneManager *sceneManager,
                       QWidget *parent = 0);

    virtual ~DuiWindow();

    /*!
     * \brief Sets the translucency of DuiWindow
     *
     * Having a translucent background means that any window behind it will be
     * shown.
     *
     * Calling this method is costly since it involves recreating the viewport.
     * This method usually should be called only once, before showing the window.
     *
     * Background translucency is disabled by default.
     *
     *  Platform notes:
     *   - Device: This feature requires that a compositing manager is running
     *     and an EGL surface with alpha channels
     *   - Scratchbox: MesaGL does not support ARGB GLX visuals. Only workaround
     *     is to run in software mode
     *
     * \param enable If true, DuiWindow will have a transparent background.
     */
    void setTranslucentBackground(bool enable);

    /*!
     * Returns a pointer to the window's DuiScene.
     */
    DuiScene *scene();

    /*!
     * \brief Assigns a scene manager to the DuiWindow
     *
     * The existing scene manager (if any) will be deleted and replaced by
     * \a sceneManager.
     *
     * DuiWindow will take ownership over \a sceneManager and use
     * the scene that is managed by it.
     *
     * \param sceneManager scene manager to be used. Can be 0.
     */
    void setSceneManager(DuiSceneManager *sceneManager);

    /*!
     * Returns a pointer to the scene manager responsible for positioning scene windows
     * in the window's DuiScene.
     *
     * \return scene manager being used or 0 (zero) if none was assigned.
     */
    DuiSceneManager *sceneManager() const;

    /*!
     * Returns a pointer to the scene manager responsible for positioning scene windows
     * in the window's DuiScene.
     *
     * Non-const version. A scene manager will be automatically created if none
     * was previously set.
     */
    DuiSceneManager *sceneManager();

    /*!
     * Returns the current orientation of the window's scene.
     */
    Dui::Orientation orientation() const;

    /*!
     * Returns the current orientation angle of the window's scene.
     */
    Dui::OrientationAngle orientationAngle() const;

    /*!
     * Returns the visible size of the window (being also the size of its scene)
     * for the given \a orientation.
     */
    QSize visibleSceneSize(Dui::Orientation) const;

    /*!
     * Returns the visible size of the window (being also the size of its scene).
     */
    QSize visibleSceneSize() const;

    /*!
     * \deprecated Since 0.18.7. Use isOrientationLocked() instead.
     * \sa isOrientationLocked()
     */
    bool keepCurrentOrientation() const;

    /*!
     * \deprecated Since 0.18.7. Use setOrientationLocked() instead.
     * \sa setOrientationLocked()
     */
    void setKeepCurrentOrientation(bool enabled);

    /*!
     * Returns true if the window orientation angle is not allowed to change
     * when device orientation changes, otherwise returns false.
     * The default value is false.
     *
     * \sa setOrientationAngleLocked()
     */
    bool isOrientationAngleLocked() const;

    /*!
     * Allows to control whether the window orientation angle should be updated with device orientation
     * changes. If you need your window to be displayed only in certain orientation angle,
     * set the desired angle using setOrientationAngle() and set this property to true.
     *
     * \example Locking window orientation angle
     *
     * \code
     *    DuiApplication app;
     *    DuiWindow window;
     *
     *    // Set the correct orientation angle.
     *    // Disable animations because we don't need them at startup.
     *    if (window.orientation() != Dui::Angle270)
     *        window.setOrientationAngle(Dui::Angle270);
     *    window.setOrientationAngleLocked(true);
     *
     *    window.show();
     *
     *    return app.exec();
     * \endcode
     *
     * \sa lockOrientationAngle(), unlockOrientationAngle()
     */
    void setOrientationAngleLocked(bool locked);

    /*!
     * Returns true if the window orientation is not allowed to change
     * when device orientation changes, otherwise returns false.
     * The default value is false.
     *
     * \sa setOrientationLocked()
     */
    bool isOrientationLocked() const;

    /*!
     * Allows to control whether the window orientation should be updated with device orientation
     * changes. If you need your window to be displayed only in certain orientation,
     * set the desired angle using setOrientationAngle() and set this property to true.
     *
     * \note Setting this property to true locks orientation changes,
     *       but allows automatic adjustments of the angle for the given orientation.
     *       This means that a window locked in portrait mode will adjust between
     *       Dui::Angle90 and Dui::Angle270 to keep "top edge" always on top and avoid
     *       upside-down situations. In order to lock specific orientation angle, use
     *       setOrientationAngleLocked().
     *
     * \example Locking window orientation
     *
     * \code
     *    DuiApplication app;
     *    DuiWindow window;
     *
     *    // Set the correct orientation angle.
     *    // Disable animations because we don't need them at startup.
     *    if (window.orientation() != Dui::Portrait)
     *        window.setOrientationAngle(Dui::Angle270);
     *    window.setOrientationLocked(true);
     *
     *    window.show();
     *
     *    return app.exec();
     * \endcode
     *
     * \sa lockOrientationAngle(), unlockOrientationAngle()
     */
    void setOrientationLocked(bool locked);

    //! Returns the current visibility state of the window.
    virtual bool isOnDisplay() const;

public Q_SLOTS:
    /*!
     * Allows to set the orientation angle of the window. If the window is hidden or it hasn't been
     * shown yet, the orientation angle change is immediate (i.e. without rotation animation).
     * Otherwise a rotation animation is performed.
     *
     * \param angle New orientation angle for the window
     */
    void setOrientationAngle(Dui::OrientationAngle angle);

    /*!
     * Locks window's orientation angle changes. Equal to calling setOrientationAngleLocked(true).
     *
     * \sa setOrientationAngleLocked(), unlockOrientationAngle()
     */
    void lockOrientationAngle();

    /*!
     * Unlocks window's orientation angle changes. Equal to calling setOrientationAngleLocked(false).
     *
     * \sa setOrientationAngleLocked(), lockOrientationAngle()
     */
    void unlockOrientationAngle();

    /*!
     * Locks window's orientation changes. Equal to calling setOrientationLocked(true).
     *
     * \sa setOrientationLocked(), unlockOrientation()
     */
    void lockOrientation();

    /*!
     * Unlocks window's orientation changes. Equal to calling setOrientationLocked(false).
     *
     * \sa setOrientationLocked(), lockOrientation()
     */
    void unlockOrientation();

Q_SIGNALS:
    /*!
     * Emitted after scene geometry has changed for a rotation.
     *
     * This is for widgets that need to react when the orientation is about to change,
     * and is emitted after the scene geometry has changed and the rotation animation
     * is about to start.
     *
     * \note This signal is emitted at start of the rotation animation.
     * \sa orientationChanged()
     */
    void orientationAngleChanged(Dui::OrientationAngle);

    /*!
     * Emitted after scene geometry has changed for a rotation.
     *
     * This is for widgets that need to react when the orientation is about to change,
     * and is emitted after the scene geometry has changed and the rotation animation
     * is about to start.
     *
     * \note This signal is emitted at start of the rotation animation.
     * \sa orientationAngleChanged()
     */
    void orientationChanged(Dui::Orientation);

    /*!
     * This signal is emitted when the rotation animation (if any) has finished.
     * It's emitted in tandem with DuiSceneManager::orientationChangeComplete().
     *
     * If window doesn't have the scene manager, this signal is emitted together with
     * orientationChanged().
     *
     * \sa orientationChanged()
     */
    void orientationChangeFinished(Dui::Orientation);

    /*!
     * A signal that is emitted when the window is shown or is not obscured anymore by another window.
     * Note!: this is different from Qt's visibilityChanged(), which is emitted due to show() and hide().
     */
    void displayEntered();

    /*!
     * A signal that is emitted when the window gets obscured by another window.
     * Note!: this is different from Qt's visibilityChanged(), which is emitted due to show() and hide().
     */
    void displayExited();

protected:
    //! \reimp
    bool event(QEvent *event);
    virtual void paintEvent(QPaintEvent *event);
    //! \reimp_end

    /*!
     * This event handler allows a window to notify subscribers about
     * changes in its presence on the display. enterDisplayEvent() and exitDisplayEvent()
     * convenience handlers are called by the default implementation. DuiOnDisplayChangeEvent
     * is sent e.g. if the window gets obscured by another window.
     */
    virtual void onDisplayChangeEvent(DuiOnDisplayChangeEvent *event);

    /*!
     * A handler that is called when the window is shown or is not obscured anymore by another window.
     * Note!: this is different from Qt's visibilityChanged(), which is emitted due to show() and hide().
     */
    virtual void enterDisplayEvent();

    /*!
     * A handler that is called when the window gets obscured by another window.
     * Note!: this is different from Qt's visibilityChanged(), which is emitted due to show() and hide().
     */
    virtual void exitDisplayEvent();

    //! \internal
    explicit DuiWindow(DuiWindowPrivate &dd, QWidget *parent = 0);
    explicit DuiWindow(DuiWindowPrivate &dd, DuiScene *scene, QWidget *parent = 0);
    explicit DuiWindow(DuiWindowPrivate &dd, DuiSceneManager *sceneManager, QWidget *parent = 0);

    DuiWindowPrivate *const d_ptr;
    //! \internal_end

    //! \reimp
    virtual void setVisible(bool visible);

private:
    Q_DISABLE_COPY(DuiWindow)
    Q_DECLARE_PRIVATE(DuiWindow)

#ifdef UNIT_TEST
    // to call orientationAngleChanged()
    friend class Ut_DuiWindow;
#endif
};

#endif
