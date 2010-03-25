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

#ifndef DUISCENEWINDOW_H
#define DUISCENEWINDOW_H

#include <duiwidgetcontroller.h>
#include <duiscenewindowmodel.h>

class DuiDismissEvent;
class DuiWindow;
class DuiSceneWindowPrivate;

/*!
 * \class DuiSceneWindow
 * \brief DuiSceneWindow objects are the base graphical items in a Direct UI scene.
 *
 * All graphical components of a standard Direct UI application are held in
 * a DuiSceneWindow of some type. DuiSceneWindow instances form the base
 * level of a Direct UI application's scene graph.
 *
 * DuiSceneWindows in a DuiScene are analogous to top level windows in a
 * traditional windowing system.
 *
 * The actual size and position of a DuiSceneWindow are by default managed by
 * DuiSceneManager according to DuiSceneWindowStyle properties. If you want to
 * manually resize and position a DuiSceneWindow you have to explicitly set
 * the managedManually property to true.
 *
 * \sa DuiSceneManager
 */
class DUI_EXPORT DuiSceneWindow : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiSceneWindow)

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
        ApplicationPage = 0,             // DuiApplicationPage
        NavigationBar,                   // DuiNavigationBar
        EscapeButtonPanel,               // DuiEscapeButtonPanel
        DockWidget,                      // DuiDockWidget
        LayerEffect,                     // DuiSceneLayerEffect
        Dialog,                          // DuiDialog
        MessageBox,                      // DuiMessageBox
        ApplicationMenu,                 // DuiApplicationMenu
        ObjectMenu,                      // DuiObjectMenu
        ModalSceneWindow,                // DuiModalSceneWindow
        PopupList,                       // DuiPopupList
        NotificationInformation,         // DuiNotification - Information
        NotificationEvent,               // DuiNotification - Event
        Overlay,                         // DuiOverlay
        Completer,                       // DuiCompleter
        HomeButtonPanel,                 // DuiHomeButtonPanel
        PlainSceneWindow,                // DuiSceneWindow
        StatusBar                        // DuiStatusBar
    };

    /*!
     * Creates an empty scene window. It's a fullscreen container of the
     * lowest Z value, that can be a parent for its child widgets and layouts.
     * \note You normally wouldn't want to instantiate this class directly and
     *       instead use one of the provided subclasses.
     */
    explicit DuiSceneWindow(QGraphicsItem *parent = 0);

    /*!
     * Destructor of the DuiSceneWindow class.
     */
    virtual ~DuiSceneWindow();

public Q_SLOTS:
    /*!
     * Shows the scene window on the window specified by \a window and registers
     * it in the associated DuiSceneManager. Uses animation to show the window.
     * \param window    The window on which the scene window is going to be shown.
     * \param policy    Deletion policy, defines the ownership for this window
     */
    virtual void appear(DuiWindow *window, DuiSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * Shows the scene window on the currently active window and registers
     * it in the associated DuiSceneManager. Uses the animation to show the window.
     * \param policy    Deletion policy, defines the ownership for this window
     *
     * \sa DuiApplication::activeWindow()
     */
    virtual void appear(DuiSceneWindow::DeletionPolicy policy = KeepWhenDone);

    /*!
     * Hides this window and unregisters it from the DuiSceneManager.
     * Uses the associated animation to hide the window.
     */
    virtual void disappear();

    /* !
     * \brief Dismisses the scene window.
     *
     * Returns true if the scene window was dismissed; otherwise returns false.
     * This slot will first send a DuiDismissEvent to the widget, which may or may
     * not accept the event. If the event was ignored, nothing happens. If the event
     * was accepted, it will disappear() the scene window.
     *
     * If DestroyWhenDone was used on the last appear() call and
     * the event was accepted, the scene window will be deleted after its
     * disappearance is finished.
     *
     * Please refer to DuiDismissEvent documentation for more information.
     *
     * \sa dismissNow(), dismissEvent(), disappear(), deleteWhenDone()
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
    DuiSceneWindow::WindowType windowType() const;

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
    DuiSceneWindow(DuiSceneWindowPrivate *dd, DuiSceneWindowModel *model,
                   DuiSceneWindow::WindowType windowType,
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
     * \brief Event handler for DuiDismissEvent
     *
     * This event handler can be reimplemented in a subclass to receive scene window
     * dismiss events. The default implementation accepts the \a event.
     *
     * \sa dismiss()
     */
    virtual void dismissEvent(DuiDismissEvent *event);

    /*!
     * Default implementation calls dismiss() and ignores the event.
     * Reimplement in a subclass to specify a different behavior
     * \sa QGraphicsWidget::closeEvent()
     */
    virtual void closeEvent(QCloseEvent *event);

    //! \reimp
    virtual bool event(QEvent *e);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(DuiSceneWindow)

    friend class DuiSceneWindowView;
    friend class DuiSceneManagerPrivate;

    /* TODO: temporarily required for calling DuiSceneWindow::done()
       from DuiSceneManager::{hide,show}Window{,Now}()*/
    friend class DuiSceneManager;
};

#endif
