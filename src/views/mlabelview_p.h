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

#ifndef MLABELVIEW_P_H
#define MLABELVIEW_P_H

#include <QTextOption>
#include <QTextDocument>
#include <QStaticText>
#include "private/mwidgetview_p.h"

class MLabel;
class MLabelView;
class QGraphicsSceneResizeEvent;
class QGestureEvent;
class QTapAndHoldGesture;
class QTimer;

class MLabelViewSimple
{
public:
    MLabelViewSimple(MLabelViewPrivate *viewPrivate);
    virtual ~MLabelViewSimple();

    virtual void drawContents(QPainter *painter, const QSizeF &size);
    virtual bool resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setupModel();
    virtual bool updateData(const QList<const char *>& modifications);
    virtual bool isRich();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void longPressEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);
    virtual void applyStyle();

    void initializeTextProperties();

    /**
     * \param width Available width in pixels for the text.
     * \return      Text that should be used for the rendering. Dependent on the available
     *              width, the wrapping-policies, eliding-settings and multilength-status
     *              adjustments are done.
     */
    QString textToRender(const QSizeF &renderSize) const;

    /**
     * \param text  Text that should be rendered.
     * \param width Available width in pixels for the text to render.
     * \return      -1.0 (= unrestricted) is returned if no wrapping must be done. If
     *              a wrapping is done it is assured that the returned width is not
     *              larger than \a width.
     */
    qreal restrictedTextWidth(const QString &text, qreal width) const;

    /**
     * \return True, if the wrap-mode indicates that a wrapping should be done.
     */
    bool wrap() const;

    /**
     * \return Text flag for the current wrap-mode specified by the model.
     */
    Qt::TextFlag textFlagForWrapMode() const;

    /**
     * Helper method for initializeStaticText(): Adjusts the member variable
     * 'textOffset' dependent on the used alignment, as QStaticText does not support
     * text alignment within a bounding rectangle.
     */
    void adjustTextOffset();

    /**
     * \return The required size of the label to fit into the given width. If the width
     *         is smaller than 0 no restriction for the width is given.
     */
    QSizeF sizeForWidth(qreal width) const;

    /**
     * \return Size for the constraint \a constraint. If the width or height
     *         of the constraint is < 0 it will be replaced by QWIDGETSIZE_MAX.
     */
    QSizeF sizeForConstraint(const QSizeF &constraint) const;

    void markDirty();

    MLabelViewPrivate *viewPrivate;

    QSizeF preferredSize;
    QPointF textOffset;
    QRectF paintingRect;
    bool dirty;
    QStaticText staticText;
    QString unconstraintText;

    bool clip;
    QPen pen;
    QFont font;
};

class MLabelViewRich : public MLabelViewSimple
{

public:
    /**
     * Caches parts of the textdocument inside a pixmap to
     * increase the performance. Several tiles might be used
     * as there are size limitations for pixmaps.
     */
    struct Tile {
        qreal y;
        QString pixmapCacheKey;
        QSize size;
    };

    MLabelViewRich(MLabelViewPrivate *viewPrivate);
    virtual ~MLabelViewRich();

    virtual void drawContents(QPainter *painter, const QSizeF &size);
    virtual bool shouldElide() const;
    virtual bool resizeEvent(QGraphicsSceneResizeEvent *event);
    virtual QSizeF sizeHint(Qt::SizeHint which, const QSizeF &constraint = QSizeF()) const;
    virtual void setupModel();
    virtual bool updateData(const QList<const char *>& modifications);
    virtual bool isRich();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void longPressEvent(QGestureEvent *event, QTapAndHoldGesture* gesture);
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);

    virtual void applyStyle();

    void ensureDocumentIsReady();
    int cursorPositionOfLastVisibleCharacter();
    void updateRichTextEliding();

    void updateHighlighters();

    /**
     * Updates the textformat to show parts of the text highlighted dependent on
     * the attached MLabelHighlighters.
     */
    void updateHighlighting();

    QString wrapTextWithSpanTag(const QString &text) const;

    /**
     * Assures that the tiles are initialized for the given size.
     * If enough pixmap cache is available the member 'tiles'
     * represents the list of all tiles.
     */
    void initTiles(const QSize &size);

    /**
     * Creates \a count tiles with a size of \a size
     * and applies them to the member 'tiles'.
     */
    void createTiles(int count, const QSize &size);

    /**
     * Releases the cache for all tiles. The member 'tiles' will be set empty.
     */
    void cleanupTiles();

    /**
     * \param painter Painter where the tiles are drawn into.
     * \param pos     Position of the tile relative to the painter.
     * \param size    Size of the MLabelView widget.
     */
    void drawTiles(QPainter *painter, const QPointF &pos, const QSizeF &size);

    /**
     * If the scene-position has been changed the vertical position of the
     * tiles (stored in the member 'tiles') probably must be adjusted if the
     * tile got completely invisible.
     */
    void updateTilesPosition();

    /**
     * Updates the pixmap of the tile with the content of the textdocument
     * from the position indicated by the tile.
     * \return False, if the updating has been failed because of a limited cache.
     */
    bool updateTilePixmap(const Tile &tile);

    /**
     * \return True, if all tiles indicated by the member 'tiles' are cached
     *         inside QPixmapCache.
     */
    bool isTilesCacheValid() const;

    Tile* topTile();
    Tile* bottomTile();

    /**
     * \param x Output parameter that specifies the x-value for the QPainter
     *          translation that is required for a correct padding in the
     *          right-to-left mode.
     * \return  Boundaries of the text including the left padding of the style.
     *          If a right-to-left alignment is used the boundaries are adjusted
     *          to clip the left part of the text in case if there is not enough
     *          width available.
     */
    QRectF textBoundaries(qreal *x) const;

    /**
     *\return Default stylesheet string that defines the text-color and anchor-color.
     */
    QString defaultStyleSheet() const;

    /**
     * Triggers an asynchronous update of the text-highlighting. It is important
     * to do the update asynchronously because MLabelViewRich::updateHighlighting() is a
     * quite expensive operation. Multiple calls of MLabelViewRich::triggerHighlightingUpdate()
     * within one callstack will only result in one call of MLabelViewRich::updateHighlighting().
     */
    void triggerHighlightingUpdate();

    mutable QTextDocument textDocument;
    bool textDocumentDirty;
    QPoint pixmapOffset;
    int mouseDownCursorPos;

    int tileHeight;
    QString tileCacheKey;
    M::Orientation tileOrientation;
    QList<Tile> tiles;

    bool highlightersChanged;
    bool isElided;
};

class MLabelViewPrivate : public MWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(MLabelView)

public:
    MLabelViewPrivate();
    virtual ~MLabelViewPrivate();

    const MLabelModel *model() const;
    const MLabelStyle *style() const;
    const QRectF boundingRect() const;

    /**
     * Triggers a call of MLabelViewSimple::handleNotification() after the specified
     * interval. If the method is invoked before the interval has been exceeded, the
     * remaining interval will get reset to the specified interval.
     */
    void requestHighlighterUpdate(int interval);

    void _q_highlighterUpdateTimerExceeded();

    //Should label be rendered as rich text
    bool displayAsRichText(QString text, Qt::TextFormat textFormat, int numberOfHighlighters) const;

    // Set the text direction and alignment in textOptions according to the text direction in
    // the model or (if auto) based on the model text's content
    void autoSetTextDirection();

    // need define this for there are overload functions in controller
    MLabel *controller;

    MLabelViewSimple *impl;
    QTextOption textOptions;

    QSizeF paddedSize;
    QTimer *highlighterUpdateTimer;
};

#endif
