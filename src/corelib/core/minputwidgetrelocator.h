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

#ifndef MINPUTWIDGETRELOCATOR_H
#define MINPUTWIDGETRELOCATOR_H

#include "mnamespace.h"
#include "mscenewindow.h"
#include <QObject>
#include <QPointer>
#include <QRect>

class MPannableViewport;
class MRelocatorStyleContainer;
class MSceneManager;
class MScrollChain;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsWidget;

enum PostponeRelocationFlag {
    WaitForRotationFinished = 0x1,
    WaitForAnimationsFinished = 0x2
};
Q_DECLARE_FLAGS(PostponeRelocationFlags, PostponeRelocationFlag)

//! \internal

/*!
 * \brief Relocates focused input widget to a visible place in scene, for editing and viewing.
 *
 * There can be only one instance of relocator per scene.
 */
class MInputWidgetRelocator : public QObject
{
    Q_OBJECT
public:
    explicit MInputWidgetRelocator(MSceneManager &sceneManager,
                                   const QGraphicsItem *rootElement,
                                   M::Orientation initialOrientation);
    virtual ~MInputWidgetRelocator();

public slots:
    /*!
     * \brief Ensures current focused widget is visible in the scene.
     *
     *  Relocation of the widget is done with the help of its parent widgets that has
     *  the capability to do it.
     *  May show or hide window decorations.
     */
    void update();

    /*!
     * \brief Setter for current application page in the scene, if any.
     */
    void setCurrentPage(const QPointer<MSceneWindow> &page);

    /*!
     * \brief Must be called on beginning of device orientation angle change.
     *
     *  Should be called before scene windows' geometries are updated.
     *  Together with \a handleRotationFinished blocks relocation requests
     *  while rotation is in progress.
     */
    void handleRotationBegin(M::Orientation orientation);

    /*!
     * \brief Must be called in the end of device orientation angle change.
     *
     * If needed and possible for the moment being, calling this method triggers relocation.
     * \sa handleRotationBegin
     */
    void handleRotationFinished();

    /*!
     * \brief This tells relocator whether the new state of scene window.
     *
     *  Relocation is blocked during the time any scene window is in potentially animated
     *  state, i.e. appearing or disappearing. Fading animations do not cause trouble but
     *  sliding animations etc will. There is no point in relocating while input widget
     *  is being moved.
     *  If needed and possible for the moment being, after animations have been finished
     *  the relocation is triggered.
     */
    void sceneWindowStateHasChanged(MSceneWindow *, MSceneWindow::SceneWindowState newState,
                                                    MSceneWindow::SceneWindowState oldState);

private slots:
    void handleKeyboardStateChange();
    void handleInputMethodAreaChange(const QRect &rect);

private:
    const MRelocatorStyleContainer &style() const;

    //! \brief Gets area of the scene where the input widget can in theory be moved to.
    //! The rect is returned in rotated scene coordinates.
    const QRect &exposedContentRect();

    bool needsMoreScreenArea() const;
    void updateScreenArea();
    void maximizeScreenArea();
    void restoreScreenArea();

    QGraphicsWidget *focusedWidget() const;

    //! Micro focus rectangle of the input widget in its local coordinates.
    QRect microFocusRect(const QGraphicsWidget *inputWidget) const;

    //! Scene rectangle in rotated scene coordinates.
    QRect visibleSceneRect() const;

    //! Returns scene window ancestor of \a child if it contains a toolbar. Otherwise returns NULL.
    const MSceneWindow *toolbarParentSceneWindow(const QGraphicsWidget *child) const;

    bool needsRelocation(const QGraphicsWidget *inputWidget,
                         const QRect &localRect);

    bool isWidgetRectFullyVisible(const QGraphicsWidget *widget,
                                  const QRect &localRect) const;

    //! If chain contains a dockable widget, dock it and return true.
    //! Otherwise return false.
    bool scrollDockedWidget(MScrollChain *chain,
                            const QGraphicsWidget *inputWidget,
                            const QRect &microRect);

    void centerContextWidgetToAnchorPoint(MScrollChain *newChain,
                                          const QPoint &anchorPoint,
                                          const QGraphicsWidget *inputWidget);

    void clearPostponeRelocationFlag(PostponeRelocationFlag flag);

private:
    const QGraphicsScene * const scene;
    const QGraphicsItem * const rootElement;
    M::Orientation orientation;
    QPointer<MSceneWindow> currentPage;

    QRect inputPanelRect;
    QRect cachedExposedRect;

    MScrollChain *oldChain;

    bool relocating;
    bool updatePending;

    int numOfDisappearingSceneWindows;
    int numOfAppearingSceneWindows;

    PostponeRelocationFlags postponeFlags;

    QScopedPointer<MRelocatorStyleContainer> styleContainer;
};

//! \internal_end

#endif // MINPUTWIDGETRELOCATOR_H
