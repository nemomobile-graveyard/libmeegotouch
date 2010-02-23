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

#ifndef DUISCENEMANAGER_H
#define DUISCENEMANAGER_H

#include "duiexport.h"

#include <QObject>
#include <QPointF>
#include <QSize>
#include <QList>
#include <DuiSceneWindow>

#include "duinamespace.h"

class DuiSceneManagerPrivate;
class DuiScene;
class DuiSceneLayerEffect;
class DuiDialog;
class QGraphicsWidget;

/*!
 * \class DuiSceneManager
 * \brief DuiSceneManager manages the DuiSceneWindows present in a DuiScene.
 *
 * DuiSceneManager ensures that the DuiSceneWindows of a DuiScene are correctly
 * positioned and stacked, similarly to what a window manager does for top level
 * windows in a tradicional windowing system.
 *
 * DuiSceneManager also orchestrates DuiSceneWindow's transitions such as
 * appearance, disappearance and display orientation changes (e.g., from portrait
 * to landscape).
 *
 * \sa DuiSceneWindow
 */
class DUI_EXPORT DuiSceneManager : public QObject
{
    Q_OBJECT
    friend class DuiSceneWindow;
#ifdef UNIT_TEST
    friend class Ut_DuiSceneManager;
#endif

public:

    /*!
     * This enum is used to describe whether the orientation change
     * invoked manually should be animated or not.
     *
     * \sa setOrientationAngle()
     */
    enum OrientationChangeMode {
        AnimatedOrientationChange,
        ImmediateOrientationChange
    };

    /*!
     * Constructor of the DuiSceneManager class, constructs the manager for the given \a scene.
     *
     * \note You normally don't have to create an instance of this class,
     *       the DuiApplicationWindow class already provides the scene manager.
     *
     * \param scene Scene to be used. If 0, scene manager will automatically
     *              create his own DuiScene.
     *
     */
    DuiSceneManager(DuiScene *scene = 0, QObject *parent = 0);

    /*!
     * Destructor of the DuiSceneManager class.
     */
    virtual ~DuiSceneManager();

    /*!
     * Get scene instance that was created by this scene manager.
     * \return Returns the scene that is managed by this scene manager.
     */
    DuiScene *scene();

    /*!
     * Returns the current orientation. It's a convenience method with
     * which you can get the orientation without querying the application window.
     */
    Dui::Orientation orientation() const;

    /*!
     * Returns the current orientation angle. It's a convenience method with
     * which you can get the orientation angle without querying the application window.
     */
    Dui::OrientationAngle orientationAngle() const;

    /*!
     * Returns the visible scene size in \a orientation. It's a convenience method with
     * which you can get the scene size without querying the application window.
     */
    QSize visibleSceneSize(Dui::Orientation orientation) const;

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
    QList<DuiSceneWindow *> pageHistory() const;

    /*!
     * \brief Sets the list of application pages that comprises the navigation history.
     *
     * By getting the current history with pageHistory(), modifying it, and
     * then feeding the modified version back to the scene manager with setPageHistory()
     * an application can freely manipulate its page navigation history.
     *
     * \sa pageHistory(), pageHistoryChanged()
     */
    void setPageHistory(const QList<DuiSceneWindow *> &list);

public Q_SLOTS:
    /*!
     * Sets the orientation to \a angle. The \a mode can be set to ImmediateOrientationChange
     * to disable orientation animation.
     *
     * /sa OrientationChangeMode
     */
    void setOrientationAngle(Dui::OrientationAngle angle,
                             OrientationChangeMode mode = AnimatedOrientationChange);

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
     * Attaches a \a window to the scene manager and shows it using associated animation.
     * According to the given \a policy, a window can be kept or destroyed after hiding.
     *
     * \note Normally you don't have to call this method explicitly. DuiSceneWindow::appear()
     * calls this method for you.
     */
    void showWindow(DuiSceneWindow *window, DuiSceneWindow::DeletionPolicy policy = DuiSceneWindow::KeepWhenDone);

    /*!
     * Attaches a \a window to the scene manager and shows it without animations (instantly).
     * According to the given \a policy, a window can be kept or destroyed after hiding.
     *
     * \note Normally you don't have to call this method explicitly. DuiSceneWindow::appearNow()
     * calls this method for you.
     */
    void showWindowNow(DuiSceneWindow *window, DuiSceneWindow::DeletionPolicy policy = DuiSceneWindow::KeepWhenDone);

    /*!
     * Shows a modal \a dialog using associated animation and returns its result code.
     *
     * \note Normally you don't have to call this method explicitly.
     * DuiDialog::exec() calls this method for you.
     */
    int execDialog(DuiDialog *dialog);

    /*!
     * Hides a \a window using associated animation and detaches it from the scene manager.
     *
     * \note Normally you don't have to call this method explicitly. DuiSceneWindow::disappear()
     * calls this method for you.
     */
    void hideWindow(DuiSceneWindow *window);

    /*!
     * Hides a \a window without animations (instantly) and detaches it from the scene manager.
     *
     * \note Normally you don't have to call this method explicitly. DuiSceneWindow::disappearNow()
     * calls this method for you.
     */
    void hideWindowNow(DuiSceneWindow *window);

    /*!
     * Closes a \a window using associated animation and detaches it from the scene manager.
     *
     * \note Normally you don't have to call this method explicitly. DuiSceneWindow::dismiss()
     * calls this method for you.
     */
    void closeWindow(DuiSceneWindow *window);

    /*!
     * Closes a \a window without animations (instantly) and detaches it from the scene manager.
     *
     * \note Normally you don't have to call this method explicitly. DuiSceneWindow::dismissNow()
     * calls this method for you.
     */
    void closeWindowNow(DuiSceneWindow *window);

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
    void orientationAboutToChange(Dui::Orientation orientation);

    /*! \brief Signal emitted after scene geometry has changed for a rotation
     *
     * In comparison to orientationChanged(), this signal is emmitted with every change
     * of the orientation angle. Note, that orientationChanged() won't be emitted when
     * changing e.g. from Dui::Angle180 to Dui::Angle0.
     *
     * Note that this is emitted at the start of the rotation animation.
     *
     * \param orientation New orientation of the viewport
     */
    void orientationAngleChanged(Dui::OrientationAngle orientationAngle);

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
    void orientationChanged(Dui::Orientation orientation);

    /*!
     * This signal is emitted when the rotation animation has finished.
     *
     * \param orientation New orientation of the viewport
     */
    void orientationChangeFinished(Dui::Orientation orientation);

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
    DuiSceneManagerPrivate *const d_ptr;
    //! \internal_end

    //! \reimp
    virtual bool eventFilter(QObject *watched, QEvent *event);
    //! \reimp_end

private:
    Q_DECLARE_PRIVATE(DuiSceneManager)

    Q_PRIVATE_SLOT(d_func(), void _q_windowShowAnimationFinished())
    Q_PRIVATE_SLOT(d_func(), void _q_windowHideAnimationFinished())
    Q_PRIVATE_SLOT(d_func(), void _q_setSenderGeometry())
    Q_PRIVATE_SLOT(d_func(), void _q_changeGlobalOrientation())
    Q_PRIVATE_SLOT(d_func(), void _q_emitOrientationChangeFinished())
    Q_PRIVATE_SLOT(d_func(), void _q_restoreSceneWindow())
    Q_PRIVATE_SLOT(d_func(), void _q_relocateWindowByInputPanel(const QRect &inputPanelRect))
    Q_PRIVATE_SLOT(d_func(), void _q_inputPanelOpened())
    Q_PRIVATE_SLOT(d_func(), void _q_inputPanelClosed())
    Q_PRIVATE_SLOT(d_func(), void _q_unFreezeUI())
    Q_PRIVATE_SLOT(d_func(), void _q_applyQueuedSceneWindowTransitions())
};

#endif
