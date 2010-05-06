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
 * \brief MSceneWindow objects are the base graphical items in a Direct UI scene.
 *
 * All graphical components of a standard Direct UI application are held in
 * a MSceneWindow of some type. MSceneWindow instances form the base
 * level of a Direct UI application's scene graph.
 *
 * MSceneWindows in a MScene are analogous to top level windows in a
 * traditional windowing system.
 *
 * The actual size and position of a MSceneWindow are by default managed by
 * MSceneManager according to MSceneWindowStyle properties. If you want to
 * manually resize and position a MSceneWindow you have to explicitly set
 * the managedManually property to true.
 *
 * \sa MSceneManager
 */
class M_EXPORT MSceneWindow : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MSceneWindow)

    Q_PROPERTY(bool managedManually READ isManagedManually WRITE setManagedManually)

public:
    /*!
     * This enum defines how to handle scene window after hiding it using disappear() or dismiss().
     */
    enum DeletionPolicy {
        KeepWhenDone,    //!< Window is kept alive after being dismissed or disappeared
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
        StatusBar                        // MStatusBar
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
     * Makes the scene window appear on \a window and registers it with the associated
     * MSceneManager.
     *
     * \param window    The window on which the scene window is going to be shown.
     * \param policy    Deletion policy, defines the ownership for this window
     *
     * \sa MSceneManager::appearSceneWindow()
     */
    virtual void appear(MWindow *window, MSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * Makes the scene window appear on the currently active window and registers
     * it with the associated MSceneManager.
     * \param policy    Deletion policy, defines the ownership for this window
     *
     * \sa MApplication::activeWindow(), MSceneManager::appearSceneWindow()
     */
    virtual void appear(MSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * Makes this scene window disappear and unregisters it from the associated
     * MSceneManager.
     *
     * This is the same as calling:
     * <code>sceneWindow->sceneManager()->disappearSceneWindow(sceneWindow);</code>
     *
     * \sa MSceneManager::disappearSceneWindow()
     */
    virtual void disappear();

    /* !
     * \brief Dismisses the scene window.
     *
     * Returns true if the scene window was dismissed; otherwise returns false.
     * This slot will first send a MDismissEvent to the widget, which may or may
     * not accept the event. If the event was ignored, nothing happens. If the event
     * was accepted, it will disappear() the scene window.
     *
     * If DestroyWhenDone was used on the last appear() call and
     * the event was accepted, the scene window will be deleted after its
     * disappearance is finished.
     *
     * Please refer to MDismissEvent documentation for more information.
     *
     * \sa dismissNow(), dismissEvent(), disappear(), deleteWhenDone(),
     *     MSceneManager::dismissSceneWindow()
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
     */
    void setManagedManually(bool managedManually);

Q_SIGNALS:
    /*!
        \brief Emitted when the scene window has finished its appearance transition.
        \sa appear()
     */
    void appeared();

    /*!
        \brief Emitted when the scene window has finished its disappearance transition.
        \sa disappear()
     */
    void disappeared();

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
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MSceneWindow)

    friend class MSceneWindowView;
    friend class MSceneManagerPrivate;

    /* TODO: temporarily required for calling MSceneWindow::done()
       from MSceneManager::{hide,show}Window{,Now}()*/
    friend class MSceneManager;
};

#endif
