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

#ifndef MSCENEWINDOW_H
#define MSCENEWINDOW_H

#include <mwidgetcontroller.h>
#include <mscenewindowmodel.h>

class MDismissEvent;
class MWindow;
class MSceneWindowPrivate;

/*!
 * \class MSceneWindow
 * \brief MSceneWindow objects are the base graphical items in a MeeGo Touch scene.
 *
 * All graphical components of a standard MeeGo Touch application are held in
 * a MSceneWindow of some type. MSceneWindow instances form the base
 * level of a MeeGo Touch application's scene graph.
 *
 * MSceneWindows in a MScene are analogous to top level windows in a
 * traditional windowing system.
 *
 * The actual size and position of a MSceneWindow are by default managed by
 * MSceneManager according to MSceneWindowStyle properties. If you want to
 * manually resize and position a MSceneWindow you have to explicitly set
 * the managedManually property to true.
 *
 * \section appear_methods Appearing and disappearing scene windows
 *
 * Scene windows introduce a new concept: They also appear and disappear
 * instead of just being shown and hidden like regular graphics items.
 *
 * Thus the family of methods appear()/disappear()/dismiss()/sceneWindowState()
 * is different from show()/hide()/close()/isVisible().
 *
 * The second group revolves around the isVisible() property which has its
 * meaning defined in the documentation of QGraphicsItem::setVisible().
 *
 * The first group is a higher level concept introduced by MSceneWindow/MSceneManager
 * and is about the scene manager taking a scene window, adding it to the scene
 * managed by him and setting its proper position, size, z value, parent item,
 * etc (usually with animated transitions).
 *
 * \section scenewindow_management Managed scene windows
 *
 * After a scene window has been added to a scene manager (through an appear() call),
 * the following attributes are controlled by that scene manager and therefore
 * should not be manually modified:
 *    - scene()
 *    - parentItem()
 *    - zValue()
 *    - rotation()
 *    - scale()
 *    - transformOriginPoint()
 *    - isVisible()
 *
 * In addition to those, if isManuallyManaged() is false, the following
 * attributes will also be under scene manager control:
 *    - pos()
 *    - size()
 *    - geometry()
 *
 * Manually changing attributes that are under the control of a scene manager
 * is not supported and can lead to unpredictable behavior.
 *
 * \sa MSceneManager
 */
class M_CORE_EXPORT MSceneWindow : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MSceneWindow)

    Q_PROPERTY(bool managedManually READ isManagedManually WRITE setManagedManually)

public:
    /*!
       \brief This enum defines how to handle scene window after hiding it using disappear() or dismiss().

        We have both DestroyWhenDone + disappear() and DestroyWhenDismissed + dismiss() to
        cover the use case where you want your scene windows to be deleted \b only when
        they are no longer in the page history.

        Example:

        When drilling down (from root page to sub page and then to sub-sub page) you
        want your previous pages (root page and sub page) to be kept alive since the
        user might go back to them by pressing a back button (which calls dismiss()
        on current page). But once the user have moved away from a page for good
        (pressed back on sub-sub page, reaching sub page), that page can be deleted
        (sub-sub page gets deleted). You will get that for free by using DestroyWhenDismissed.

        If you use DestroyWhenDone for your pages you will never be able to go back to
        them in a drill down navigational pattern as they will be deleted as soon as
        they leave the screen. If you use DestroyWhenDismissed they will only be
        deleted if they get dismissed, for instance, by the user via the back button.

        \sa appear()
     */
    enum DeletionPolicy {
        KeepWhenDone,    //!< Window is kept alive after being dismissed or disappeared.
        DestroyWhenDone,  //!< Window is destroyed after being dismissed or disappeared
        DestroyWhenDismissed //!< Window is destroyed after being dismissed
    };

    /*!
     * Defines multiple window types which will have different Z value
     */
    enum WindowType {
        ApplicationPage = 0,             // MApplicationPage
        NavigationBar,                   // MNavigationBar
        EscapeButtonPanel,               // MEscapeButtonPanel
        DockWidget,                      // MDockWidget
        LayerEffect,                     // MSceneLayerEffect
        Dialog,                          // MDialog
        MessageBox,                      // MMessageBox
        ApplicationMenu,                 // MApplicationMenu
        ObjectMenu,                      // MObjectMenu
        ModalSceneWindow,                // MModalSceneWindow
        PopupList,                       // MPopupList
        NotificationInformation,         // MNotification - Information
        NotificationEvent,               // MNotification - Event
        Overlay,                         // MOverlay
        Completer,                       // MCompleter
        HomeButtonPanel,                 // MHomeButtonPanel
        PlainSceneWindow,                // MSceneWindow
        StatusBar,                       // MStatusBar
        Sheet,                           // MSheet
        BorderDecoration                 // MBorderDecoration
    };

    /*!
     * This enum describes the possible states of a scene window.
     * \sa sceneWindowState(), sceneWindowStateChanged(), appeared(), disappeared()
     */
    enum SceneWindowState {
        Appearing, /*!< An appearance animation is running on the scene window.
                        A scene window enters this appearance state when appear()
                        is called from Disappeared state. */
        Appeared, /*!< The scene window is properly positioned in the scene.
                       It enters this state from Appearing, after its
                       appearance animation has finished. This state can also be
                       reached directly from Disappeared if the transition was immediate.
                       The appeared() signal gets emitted when this state is reached.*/
        Disappearing, /*!< A disappearance animation is running on the scene window.
                        A scene window enters this state when disappear() is called
                        from Appeared state. */
        Disappeared /*!< Initial state.
                         The scene window is outside the visible scene area and
                         therefore cannot be seen.
                         It enters this appearance state from Disappearing, after
                         its disappearance animation has finished. This state
                         might also be reached directly from Appeared if the
                         transition was immediate. The disappeared()
                         signal gets emitted when this state is reached.
                          */
    };

    /*!
     * Creates an empty scene window. It's a fullscreen container of the
     * lowest Z value, that can be a parent for its child widgets and layouts.
     * \note You normally wouldn't want to instantiate this class directly and
     *       instead use one of the provided subclasses.
     */
    explicit MSceneWindow(QGraphicsItem *parent = 0);

    /*!
     * Destructor of the MSceneWindow class.
     */
    virtual ~MSceneWindow();

public Q_SLOTS:

    /*!
     * \brief Makes the scene window appear on the given \a scene.
     *
     * \param scene    The scene on which the scene window is going to appear.
     * \param policy   Deletion policy. Defines whether this scene window should be
     *                 automatically deleted when no longer used.
     *
     * The scene window will be managed by the MSceneManager of the given \a scene.
     * Since the appearance and disappearance of scene windows is handled by a scene manager,
     * if \a scene has no scene manager this method won't have any effect.
     *
     * Ownership is transfered to \a scene.
     *
     * \note This is different from calling <code>scene->addItem(sceneWindow)</code>.
     *
     * Usage example:
     * \code
     * void AlbumPage::showSong(Song *song)
     * {
     *     MSceneWindow *songPage = new SongPage(song);
     *     songPage->appear(scene(), MSceneWindow::DestroyWhenDismissed);
     * }
     * \endcode
     *
     * \sa MSceneManager::appearSceneWindow()
     */
    void appear(QGraphicsScene *scene, MSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * \brief Makes the scene window appear on \a window.
     *
     * \param window    The window whose scene will receive the appearing scene window.
     * \param policy    Deletion policy. Defines whether this scene window should be
     *                  automatically deleted when no longer used.
     *
     * If \a window doesn't have a scene manager, one will be automatically created
     * (along with a scene) and assigned to it.
     *
     * Ownership is transfered to window->scene().
     *
     * Usage example:
     * \code
     * MWindow *window = new SomeWindow;
     * MSceneWindow *sceneWindow = new SomeSceneWindow;
     * sceneWindow->appear(window);
     * window->show();
     * \endcode
     *
     * \sa appear(QGraphicsScene *, DeletionPolicy), MSceneManager::appearSceneWindow()
     */
    virtual void appear(MWindow *window, MSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * Makes the scene window appear on the currently active window and registers
     * it with the associated MSceneManager.
     *
     * \deprecated Please use appear(QGraphicsScene) or appear(MWindow) instead.
     *
     * \attention If your application has multiple MWindows, usage of this method
     * is strongly discouraged since it can make a scene window appear in an MWindow
     * different from the intended one. That's due to the fact that it depends on
     * which MWindow happens to be active when this method is called. In such
     * scenarios use either appear(MWindow*) or appear(QGraphicsScene*).
     *
     * \param policy    Deletion policy. Defines whether this scene window should be
     *                  automatically deleted when no longer used.
     *
     * Ownership is transfered to the MScene visualized by the active MWindow
     * (MApplication::activeWindow()->scene()).
     *
     * \sa appear(QGraphicsScene*, DeletionPolicy), appear(MWindow*, DeletionPolicy),
     *     MApplication::activeWindow(), MSceneManager::appearSceneWindow()
     */
    virtual void appear(MSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * Makes this scene window disappear and unregisters it from the associated
     * MSceneManager.
     *
     * This is the same as calling:
     * <code>sceneWindow->sceneManager()->disappearSceneWindow(sceneWindow);</code>
     *
     * If DestroyWhenDone was used on the last appear() call, the scene window will
     * be deleted after its disappearance is finished. Note that nothing will happen
     * if the scene window is already in Disappeared state.
     *
     * \sa MSceneManager::disappearSceneWindow(), SceneWindowState
     */
    virtual void disappear();

    /* !
     * \brief Dismisses the scene window.
     *
     * Returns true if the scene window was dismissed; otherwise returns false.
     * This slot will first send a MDismissEvent to the widget, which may or may
     * not accept the event. If the event was ignored, nothing happens. If the event
     * was accepted, it will call MSceneManager::dismissSceneWindow().
     *
     * If DestroyWhenDismissed or DestroyWhenDone was used on the last appear()
     * call and the event was accepted, the scene window will be deleted after its
     * disappearance is finished. Note that nothing will happen if the scene window
     * is already in Disappeared state.
     *
     * Please refer to MDismissEvent documentation for more information.
     *
     * \sa dismissEvent(), disappear(), SceneWindowState, MSceneManager::dismissSceneWindow()
     */
    bool dismiss();

public:
    /*!
     * Returns the currently active deletion policy of this window.
     * \return Deletion policy of this window, KeepWhenDone as a default.
     * \sa DeletionPolicy
     */
    DeletionPolicy deletionPolicy() const;

    /*!
     * Returns the window type.
     * \sa WindowType
     */
    MSceneWindow::WindowType windowType() const;

    /*!
     * Returns true if window is managed manually, i.e.
     * scene manager doesn't care about its size and
     * position. Default value is false, i.e. window
     * is resized and positioned inside the scene by
     * the scene manager, according to its style attributes.
     */
    bool isManagedManually() const;

    /*!
     * Allows to explicitly enable/disable manual management
     * of the size and position of the window by the scene manager.
     * Manual management is disabled by default, letting
     * the scene manager adjust size and position of the window
     * accorting to its style attributes. If manual management
     * is enabled, the window can be positioned using e.g. resize(),
     * setPos() and/or setGeometry() and it's responsible itself for
     * proper adjusting its geometry when viewport orientation changes.
     *
     * Scene manager will still control other attributes such as the
     * assigned scene (scene()), the assigned parent item (parentItem())
     * and Z value (zValue()) regardless of the value of this property.
     */
    void setManagedManually(bool managedManually);

    /*!
     * \brief Returns the state of the scene window.
     * \sa sceneWindowStateChanged()
     */
    SceneWindowState sceneWindowState() const;

Q_SIGNALS:

    /*!
        \brief Emitted when the scene window enters the MSceneWindow::Appearing state.
        This means that the scene window is starting its appearance animation. From that
        point in time onwards, some or all of it might be inside the visible area of the
        scene.
     */
    void appearing();

    /*!
        \brief Emitted when the scene window enters the MSceneWindow::Appeared state.

        At this point the scene window has assumed its final position in the visible
        area of the scene.
        \sa appear()
     */
    void appeared();

    /*!
        \brief Emitted when the scene window enters the MSceneWindow::Disappearing state.
        This means that the scene window is starting its disappearance animation.
     */
    void disappearing();

    /*!
        \brief Emitted when the scene window enters the MSceneWindow::Disappeared state.

        The scene window is not longer present in the visible area of the scene.
        \sa disappear()
     */
    void disappeared();

    /*!
        \brief Emitted when the scene window state changes
        \param newState The new state of the scene window.
        \param oldState The previous state of the scene window.
     */
    void sceneWindowStateChanged(MSceneWindow::SceneWindowState newState,
                                 MSceneWindow::SceneWindowState oldState);

    /*!
     * \brief Emitted when window's style attributes have changed
     * and it needs to be repositioned by the scene manager.
     */
    void repositionNeeded();

protected:
    //! \internal
    MSceneWindow(MSceneWindowPrivate *dd, MSceneWindowModel *model,
                   MSceneWindow::WindowType windowType,
                   const QString &viewType = QString(""), QGraphicsItem *parent = 0);
    //! \internal_end

    /*!
     * \brief Returns the current alignment.
     * Alignment defines how scene manager will position the window on the screen.
     * This property has no effect if the window is being managed manually.
     * \sa isManagedManually()
     * \returns The current alignment.
     */
    Qt::Alignment alignment() const;

    /*!
     * \brief Returns the current offset.
     * \returns The current offset.
     */
    QPointF offset() const;

    /* Add ! here
     * \brief Event handler for MDismissEvent
     *
     * This event handler can be reimplemented in a subclass to receive scene window
     * dismiss events. The default implementation accepts the \a event.
     *
     * \sa dismiss()
     */
    virtual void dismissEvent(MDismissEvent *event);

    /*!
     * Default implementation calls dismiss() and ignores the event.
     * Reimplement in a subclass to specify a different behavior
     * \sa QGraphicsWidget::closeEvent()
     */
    virtual void closeEvent(QCloseEvent *event);

    //! \reimp
    virtual bool event(QEvent *e);
    virtual void tapAndHoldGestureEvent(QGestureEvent *event, QTapAndHoldGesture *gesture);
    virtual void gestureEvent(QGestureEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MSceneWindow)

    friend class MSceneWindowView;
    friend class MInputWidgetRelocator; // Needed for alignment attribute.
    friend class MSceneManagerPrivate;
    friend class MApplicationWindowPrivate;

    /* TODO: temporarily required for calling MSceneWindow::done()
       from MSceneManager::{hide,show}Window{,Now}()*/
    friend class MSceneManager;
};

Q_DECLARE_METATYPE(MSceneWindow::SceneWindowState)

#endif
