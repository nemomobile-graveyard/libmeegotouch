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
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsWidget;

enum PostponeRelocationFlag {
    WaitForRotationFinished = 0x1,
    WaitForAnimationsFinished = 0x2
};
Q_DECLARE_FLAGS(PostponeRelocationFlags, PostponeRelocationFlag)

//! \internal

class MInputWidgetRelocator : public QObject
{
    Q_OBJECT
public:
    explicit MInputWidgetRelocator(const QGraphicsScene *scene,
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
    void handleRotationBegin();

    /*!
     * \brief Must be called in the end of device orientation angle change.
     *
     * If needed and possible for the moment being, calling this method triggers relocation.
     * \sa handleRotationBegin
     */
    void handleRotationFinished(M::Orientation orientation);

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

signals:
    //! This signal requests immediate displacement of the given scene window. The signal should be
    //! connected to using Qt::DirectConnection, or similar, since after emitting it is assumed the
    //! window has been moved.
    void sceneWindowDislocationRequest(MSceneWindow *sceneWindow, const QPointF &displacement);

    //! This signal requests undoing previous displacement of a given scene window.
    void sceneWindowUndoDislocationRequest(MSceneWindow *sceneWindow);

private slots:
    void handleKeyboardStateChange();

private:
    //! Supported relocation operation types.
    enum RelocationOpType {
        RelocationByMovingWindow,
        RelocationByPanning
    };

    //! Relocation operation describes relocation made by one delegate widget.
    struct RelocationOp {
        RelocationOp(RelocationOpType type, QGraphicsWidget *delegate);

        RelocationOpType type;
        QGraphicsWidget *delegate;

        //! This and further operations should not be carried out if input widget is already visible.
        bool stopIfVisible;
    };

    const MRelocatorStyleContainer &style() const;

    //! \brief Gets area of the scene where the input widget can in theory be moved to.
    //! The rect is returned in rotated scene coordinates.
    const QRect &exposedContentRect();

    //! \brief Widgets optimal rectangle after relocating, in rotated scene coordinates.
    void relocationRectangles(const QGraphicsWidget &inputWidget, const QRect &microFocusRect,
                              QRect &targetRect, QRect &localRect);

    void ensureTopmostViewportIsPannable();
    void ensureInputWidgetVisible(const QGraphicsWidget &inputWidget);

    void buildRelocationOpList(const QGraphicsWidget &inputWidget, QList<RelocationOp> &ops,
                               bool widgetInitiallyVisible);

    void relocate(const QGraphicsWidget &inputWidget,
                  const QRect &targetRect, const QRect &localRect);
    void relocateByPannableViewport(MPannableViewport *viewport, const QRect &targetRect,
                                    const QPoint &originPoint);
    void relocateBySceneWindow(MSceneWindow *sceneWindow, const QRect &targetRect,
                               const QPoint &originPoint);

    void restoreRelocations();

    bool needsMoreScreenArea() const;
    void updateScreenArea();
    void maximizeScreenArea();
    void restoreScreenArea();

    QGraphicsWidget *focusedWidget() const;

    //! Micro focus rectangle of the input widget in rotated scene coordinates.
    QRect microFocusRect() const;

    //! Scene rectangle in rotated scene coordinates.
    QRect visibleSceneRect() const;

    void moveRectInsideArea(const QRect &area, QRect &rect) const;

    bool needsRelocating(const QGraphicsWidget &inputWidget, const QRect &localRect);

    bool isObscured(const QGraphicsWidget &widget, const QRect &localRect);

    void clearPostponeRelocationFlag(PostponeRelocationFlag flag);

private:
    const QGraphicsScene * const scene;
    const QGraphicsItem * const rootElement;
    M::Orientation orientation;
    QPointer<MSceneWindow> currentPage;

    QRect inputPanelRect;
    QRect cachedExposedRect;
    MPannableViewport *cachedTopmostPannableViewport;

    // Utilizing QObject's QPointer-awareness because these items can be destroyed before
    // we try restoring them, i.e. relying on ~QObject() to set these to null.
    QList< QPointer<MPannableViewport> > pannablesToRestore;
    QList< QPointer<MSceneWindow> > sceneWindowsToRestore;

    bool relocating;
    bool updatePending;

    int numOfDisappearingSceneWindows;
    int numOfAppearingSceneWindows;

    PostponeRelocationFlags postponeFlags;

    QScopedPointer<MRelocatorStyleContainer> styleContainer;
};

//! \internal_end

#endif // MINPUTWIDGETRELOCATOR_H
