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

#ifndef MSCENEMANAGER_H
#define MSCENEMANAGER_H

#include "mexport.h"

#include <QObject>
#include <QPointF>
#include <QSize>
#include <QList>
#include <MSceneWindow>

#include "mnamespace.h"

class MSceneManagerPrivate;
class MScene;
class MSceneLayerEffect;
class MDialog;
class QGraphicsWidget;

/*!
 * \class MSceneManager
 * \brief MSceneManager manages the MSceneWindows present in a MScene.
 *
 * MSceneManager ensures that the MSceneWindows of a MScene are correctly
 * positioned and stacked, similarly to what a window manager does for top level
 * windows in a tradicional windowing system.
 *
 * MSceneManager also orchestrates MSceneWindow's transitions such as
 * appearance, disappearance and display orientation changes (e.g., from portrait
 * to landscape).
 *
 * \sa MSceneWindow
 */
class M_EXPORT MSceneManager : public QObject
{
    Q_OBJECT
    friend class MSceneWindow;
#ifdef UNIT_TEST
    friend class Ut_MSceneManager;
#endif

public:

    /*!
     * This enum is used to describe whether the orientation change
     * invoked manually should be animated or not.
     *
     * \sa setOrientationAngle()
     */
    enum TransitionMode {
        AnimatedTransition,
        ImmediateTransition
    };

    /*!
     * Constructor of the MSceneManager class, constructs the manager for the given \a scene.
     *
     * \note You normally don't have to create an instance of this class,
     *       the MApplicationWindow class already provides the scene manager.
     *
     * \param scene Scene to be used. If 0, scene manager will automatically
     *              create his own MScene.
     *
     */
    MSceneManager(MScene *scene = 0, QObject *parent = 0);

    /*!
     * Destructor of the MSceneManager class.
     */
    virtual ~MSceneManager();

    /*!
     * Get scene instance that was created by this scene manager.
     * \return Returns the scene that is managed by this scene manager.
     */
    MScene *scene();

    /*!
     * Returns the current orientation. It's a convenience method with
     * which you can get the orientation without querying the application window.
     */
    M::Orientation orientation() const;

    /*!
     * Returns the current orientation angle. It's a convenience method with
     * which you can get the orientation angle without querying the application window.
     */
    M::OrientationAngle orientationAngle() const;

    /*!
     * Returns the visible scene size in \a orientation. It's a convenience method with
     * which you can get the scene size without querying the application window.
     */
    QSize visibleSceneSize(M::Orientation orientation) const;

    /*!
     * Returns the visible scene size in current orientation. It's a convenience method with
     * which you can get the scene size without querying the application window.
     */
    QSize visibleSceneSize() const;

    /*!
     * \brief Returns the list of application pages that comprises the current navigation history.
     *
     * The last page in the list is the previous application page, the one that
     * will automatically reappear when the current one is dismissed.
     *
     * The first page in the list is the root application page. In regular
     * navigation flows this is the first page that appeared to the user.
     *
     * \sa setPageHistory(), pageHistoryChanged()
     */
    QList<MSceneWindow *> pageHistory() const;

    /*!
     * \brief Sets the list of application pages that comprises the navigation history.
     *
     * By getting the current history with pageHistory(), modifying it, and
     * then feeding the modified version back to the scene manager with setPageHistory()
     * an application can freely manipulate its page navigation history.
     *
     * \sa pageHistory(), pageHistoryChanged()
     */
    void setPageHistory(const QList<MSceneWindow *> &list);

public Q_SLOTS:
    /*!
     * Sets the orientation to \a angle. The \a mode can be set to ImmediateTransition
     * to disable orientation animation.
     *
     * /sa TransitionMode
     */
    void setOrientationAngle(M::OrientationAngle angle,
                             TransitionMode mode = AnimatedTransition);

    /*!
     * Sends a request to the application's input context to open a software input
     * panel (e.g. the virtual keyboard) for the given \a inputWidget. Depending on
     * a placement of the \a inputWidget scene manager adjusts the scene by
     * temporarily hiding the escape button and navigation bar.
     * \param inputWidget A widget that requests the software input panel
     * \sa closeSoftwareInputPanel
     */
    void requestSoftwareInputPanel(QGraphicsWidget *inputWidget);

    /*!
     * Sends a request to the application's input context to close a software input
     * panel. Restores temporarily hidden escape button and navigation bar.
     * \sa requestSoftwareInputPanel
     */
    void closeSoftwareInputPanel();

    /*!
     * Attaches a \a sceneWindow to the scene manager and makes it appear.
     * According to the given \a policy, a scene window can be kept or destroyed after disappearing.
     *
     * If the scene is currently being displayed by any MWindow (according to
     * MWindow::isOnDisplay()), the appearance transition will be animated.
     * Otherwise, it will be immediate.
     *
     * \note Normally you don't have to call this method explicitly. MSceneWindow::appear()
     * calls this method for you.
     */
    void appearSceneWindow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy = MSceneWindow::KeepWhenDone);

    /*!
     * Attaches a \a sceneWindow to the scene manager and makes it appear without animations (instantly).
     * According to the given \a policy, a scene window can be kept or destroyed after disappearing.
     */
    void appearSceneWindowNow(MSceneWindow *sceneWindow, MSceneWindow::DeletionPolicy policy = MSceneWindow::KeepWhenDone);

    /*!
     * Makes a \a dialog appear using associated animation and returns its result code.
     *
     * \note Normally you don't have to call this method explicitly.
     * MDialog::exec() calls this method for you.
     */
    int execDialog(MDialog *dialog);

    /*!
     * Makes a \a sceneWindow disappear and detaches it from the scene manager.
     *
     * If the scene is currently being displayed by any MWindow (according to
     * MWindow::isOnDisplay()), the disappearance transition will be animated.
     * Otherwise, it will be immediate.
     *
     * \note Normally you don't have to call this method explicitly. MSceneWindow::disappear()
     * calls this method for you.
     */
    void disappearSceneWindow(MSceneWindow *sceneWindow);

    /*!
     * Makes a \a sceneWindow disappear without animations (instantly) and detaches it from the scene manager.
     */
    void disappearSceneWindowNow(MSceneWindow *sceneWindow);

    /*!
     * Dismisses a \a sceneWindow and detaches it from the scene manager.
     *
     * If the scene is currently being displayed by any MWindow (according to
     * MWindow::isOnDisplay()), the dismissal transition will be animated.
     * Otherwise, it will be immediate.
     *
     * \note Normally you don't have to call this method explicitly. MSceneWindow::dismiss()
     * calls this method for you.
     */
    void dismissSceneWindow(MSceneWindow *sceneWindow);

    /*!
     * Dismisses a \a sceneWindow without animations (instantly) and detaches it from the scene manager.
     */
    void dismissSceneWindowNow(MSceneWindow *sceneWindow);

    /*!
     * Ensures that the cursor is visible by scrolling the relevant pannable viewport
     * and/or moving the relevant scene window, if necessary.
     */
    void ensureCursorVisible();

Q_SIGNALS:
    /*! \brief Signal emitted before scene geometry is changed for a rotation
     *
     * This is for widgets that need to react when the orientation is about to change,
     * and is emitted before the scene geometry is changed.
     *
     * This is the preferred way for the widgets to
     * hook to orientation change.
     *
     * \param orientation New orientation of the viewport
     */
    void orientationAboutToChange(M::Orientation orientation);

    /*! \brief Signal emitted after scene geometry has changed for a rotation
     *
     * In comparison to orientationChanged(), this signal is emmitted with every change
     * of the orientation angle. Note, that orientationChanged() won't be emitted when
     * changing e.g. from M::Angle180 to M::Angle0.
     *
     * Note that this is emitted at the start of the rotation animation.
     *
     * \param orientation New orientation of the viewport
     */
    void orientationAngleChanged(M::OrientationAngle orientationAngle);

    /*! \brief Signal emitted after scene geometry has changed for a rotation
     *
     * This is for widgets that need to react when the orientation is about to change,
     * and is emitted after the scene geometry has changed and the rotation animation
     * is about to start.
     *
     * Note that this is emitted at the start of the rotation animation.
     *
     * This is the preferred way for the widgets to hook to orientation change.
     *
     * \param orientation New orientation of the viewport
     */
    void orientationChanged(M::Orientation orientation);

    /*!
     * This signal is emitted when the rotation animation has finished.
     *
     * \note It's emitted with every orientation angle change.
     *
     * \param orientation New orientation of the viewport
     */
    void orientationChangeFinished(M::Orientation orientation);

    /*!
     * This signal is emitted whenever the page history stack changes.
     * That can happen due to the following reasons:
     * \li The page navigation history was manually changed by a setPageHistoryStack() call.
     * \li A new page appeared (and another page was being displayed before that).
     * \li The current page was dismissed (and the history was not empty).
     *
     * \sa pageHistory(), setPageHistory()
     */
    void pageHistoryChanged();

protected:
    //! \internal
    MSceneManagerPrivate *const d_ptr;
    //! \internal_end

    //! \reimp
    virtual bool eventFilter(QObject *watched, QEvent *event);
    virtual void childEvent(QChildEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(MSceneManager)

    Q_PRIVATE_SLOT(d_func(), void _q_onSceneWindowAppeared())
    Q_PRIVATE_SLOT(d_func(), void _q_onSceneWindowDisappeared())
    Q_PRIVATE_SLOT(d_func(), void _q_setSenderGeometry())
    Q_PRIVATE_SLOT(d_func(), void _q_changeGlobalOrientationAngle())
    Q_PRIVATE_SLOT(d_func(), void _q_emitOrientationChangeFinished())
    Q_PRIVATE_SLOT(d_func(), void _q_restoreSceneWindow())
    Q_PRIVATE_SLOT(d_func(), void _q_relocateWindowByInputPanel(const QRect &inputPanelRect))
    Q_PRIVATE_SLOT(d_func(), void _q_inputPanelOpened())
    Q_PRIVATE_SLOT(d_func(), void _q_inputPanelClosed())
    Q_PRIVATE_SLOT(d_func(), void _q_unFreezeUI())
    Q_PRIVATE_SLOT(d_func(), void _q_applyQueuedSceneWindowTransitions())
    Q_PRIVATE_SLOT(d_func(), void _q_triggerAsyncPendingOrientationChange())
    Q_PRIVATE_SLOT(d_func(), void _q_applyPendingOrientationChange())
};

#endif
