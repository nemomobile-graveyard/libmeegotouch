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
    Qt::TextFlag wrap() const;
    void initializeStaticText();
    
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
    QPoint textOffset;
    QRectF paintingRect;
    bool dirty;
    QStaticText staticText;
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

    QRectF textBoundaries() const;

    mutable QTextDocument textDocument;
    bool textDocumentDirty;
    QPoint pixmapOffset;
    int mouseDownCursorPos;

    int tileHeight;
    QString tileCacheKey;
    M::Orientation tileOrientation;
    QList<Tile> tiles;
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

    bool isRichText(QString text) const;

    // need define this for there are overload functions in controller
    MLabel *controller;

    MLabelViewSimple *impl;
    QTextOption textOptions;
};

#endif
