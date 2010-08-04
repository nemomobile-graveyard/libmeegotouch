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
#include <QObject>
#include <QPointer>
#include <QRect>

class MPannableViewport;
class MSceneManager;
class MSceneWindow;
class QGraphicsItem;
class QGraphicsScene;
class QGraphicsWidget;

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

    void handleRotationBegin();
    void handleRotationFinished(M::Orientation orientation);

signals:
    //! This signal requests immediate displacement of the given scene window. The signal should be
    //! connected to using Qt::DirectConnection, or similar, since after emitting it is assumed the
    //! window has been moved.
    void sceneWindowDislocationRequest(MSceneWindow *sceneWindow, const QPointF &displacement);

    //! This signal requests undoing previous displacement of a given scene window.
    void sceneWindowUndoDislocationRequest(MSceneWindow *sceneWindow);

private:
    //! \brief Gets area of the scene where the input widget can in theory be moved to.
    //! The rect is returned in rotated scene coordinates.
    const QRect &exposedContentRect();

    //! \brief Widgets optimal position after relocating, in rotated scene coordinates.
    QPoint targetWidgetPosition(const QGraphicsWidget &inputWidget, const QRect &exposedRect, const QRect &microFocusRect);

    void ensurePannableViewportIsPannable(const QGraphicsWidget &inputWidget);
    void ensureInputWidgetVisible(const QGraphicsWidget &inputWidget);
    void relocate(QGraphicsWidget *delegate, const QPoint &contentsOffset, bool isAlreadyVisible);
    void relocateByPannableViewport(MPannableViewport *viewport, const QPoint &contentsOffset);
    void relocateBySceneWindow(MSceneWindow *sceneWindow, const QPoint &contentsOffset, bool isAlreadyVisible);

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

private:
    const QGraphicsScene * const scene;
    const QGraphicsItem * const rootElement;
    M::Orientation orientation;
    QPointer<MSceneWindow> currentPage;
    QRect inputPanelRect;
    QRect cachedExposedRect;

    // Utilizing QObject's QPointer-awareness because these items can be destroyed before
    // we try restoring them, i.e. relying on ~QObject() to set these to null.
    QList< QPointer<MPannableViewport> > pannablesToRestore;
    QList< QPointer<MSceneWindow> > sceneWindowsToRestore;

    bool relocating;
    bool blockRelocationRequests;
};

//! \internal_end

#endif // MINPUTWIDGETRELOCATOR_H
