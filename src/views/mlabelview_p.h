/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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
#include "mlabel.h"
#include "private/mwidgetview_p.h"

class MLabel;
class MLabelView;
class MSceneWindow;
class QGraphicsSceneResizeEvent;
class QGestureEvent;
class QTapAndHoldGesture;
class QTimer;
class MLabelHighlighter;
class QTextCharFormat;

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

    /**
     * \return Text that gets rendered in drawContents(). The text might differ
     *         from MLabel::text() because of the eliding or adjusted linebreaks.
     *         The method is used by MLabelView::renderedText() to allow Ut_MLabel
     *         accessing the rendered text for unit testing.
     */
    virtual QString renderedText() const;

    /**
     * The method is used by MLabelView::tileInformation() to allow Ut_MLabel
     * accessing this information for unit testing.
     *
     * \param pixmap Output parameter that contains the tile-pixmap if the
     *               the return value has been true.
     * \param pos    Output parameter that returns the position of the tile
     *               if the return value has been true.
     * \return       True if accessing the tiles is possible. If false has been
     *               returned the tiling had been disabled because of e.g. a
     *               cleared QPixmapCache.
     */
    virtual bool tileInformation(int index, QPixmap &pixmap, QPoint &pos) const;

    void initializeTextProperties();

    /**
     * \param renderSize Available size in pixels for the text.
     * \param isMultipleLines This is set to whether the text will take up more than one line
     * \return      Text that should be used for the rendering. Dependent on the available
     *              width, the wrapping-policies, eliding-settings and multilength-status
     *              adjustments are done.
     */
    QString textToRender(const QSizeF &renderSize, bool *isMultipleLines) const;

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
    QSizeF sizeForWidth(qreal width, const QString &text, int maximumLineCount, int preferredLineCountBehavior) const;

    /**
     * \return Size for the constraint \a constraint. If the width or height
     *         of the constraint is < 0 it will be replaced by QWIDGETSIZE_MAX.
     */
    QSizeF sizeForConstraint(const QSizeF &constraint) const;

    /** Split the model->text() into variants, and replaces spaces with newlines */
    void updateStringVariants();
    void markDirty();

    MLabelViewPrivate *viewPrivate;

    QSizeF preferredSize;
    QPointF textOffset;
    QRectF paintingRect;
    QRectF paintingRectWithMargins;
    bool dirty;
    QStaticText staticText;
    QStringList stringVariants;

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
        qreal x;
        qreal y;
        QString pixmapCacheKey;
        QSize size;
    };

    MLabelViewRich(MLabelViewPrivate *viewPrivate);
    virtual ~MLabelViewRich();

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
    virtual QString renderedText() const;
    virtual bool tileInformation(int index, QPixmap &pixmap, QPoint &pos) const;

    void ensureDocumentIsReady();
    int cursorPositionOfLastVisibleCharacter();
    void updateRichTextEliding();

    /**
     * Helper method for updateRichTextEliding(). Returns true if the text
     * should get elided considering the current width of the label.
     */
    bool shouldElide() const;

    void updateHighlighters();

    /**
     * Updates the textformat to show parts of the text highlighted dependent on
     * the attached MLabelHighlighters.
     */
    void updateHighlighting();

    QString wrapTextWithSpanTag(const QString &text) const;
    MLabelHighlighter* extractHighlighter(const QTextCharFormat& format);
    QTextCharFormat charFormat(const QPointF& pos);
    QTextCharFormat charFormat(int cursorPos);
    int cursorPosition(const QPointF& pos);

    /**
     * Assures that the tiles are initialized for the given size.
     * If enough pixmap cache is available the member 'tiles'
     * represents the list of all tiles.
     */
    void initTiles();

    /**
     * Creates \a count tiles with a size of \a size
     * and applies them to the member 'tiles'.
     */
    void createTiles(int columns, int rows, const QSize &size);

    /**
     * Releases the cache for all tiles. The member 'tiles' will be set empty.
     */
    void cleanupTiles();

    /**
     * \param painter Painter where the tiles are drawn into.
     * \param pos     Position of the tile relative to the painter.
     */
    void drawTiles(QPainter *painter, const QPointF &pos);

    /**
     * If the scene-position has been changed the vertical position of the
     * tiles (stored in the member 'tiles') probably must be adjusted if the
     * tile got completely invisible.
     */
    void updateTilesPosition();

    /**
     * \return Sum of the style's top padding, margin and reactive margin.
     */
    int topMargin() const;

    /**
     * \return Sum of the style's bottom padding, margin and reactive margin.
     */
    int bottomMargin() const;

    /**
     * \return Sum of the style's left padding, margin and reactive margin.
     */
    int leftMargin() const;

    /**
     * \return Sum of the style's right padding, margin and reactive margin.
     */
    int rightMargin() const;

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

    /**
     * Updates the maximum tile size maxTileSize to the screen size respecting
     * the orientation.
     */
    void updateMaximumTileSize(M::Orientation orientation);

    mutable QTextDocument textDocument;
    bool textDocumentDirty;
    QPoint pixmapOffset;
    int mouseDownCursorPos;

    QSize maxTileSize;
    QString tileCacheKey;
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
    QPointF mapToRoot(const QPointF &point);

    /**
     * Triggers a call of MLabelViewSimple::handleNotification() after the specified
     * interval. If the method is invoked before the interval has been exceeded, the
     * remaining interval will get reset to the specified interval.
     */
    void requestHighlighterUpdate(int interval);

    void _q_highlighterUpdateTimerExceeded();

    //Should label be rendered as rich text
    bool displayAsRichText(QString text, Qt::TextFormat textFormat, int numberOfHighlighters) const;

    // Map CSS string to an enum, for the preferred line count behavior
    MLabel::PreferredLineCountBehavior stringToLineCountBehavior(const QString &behavior) const;

    // Set the text direction and alignment in textOptions according to the text direction in
    // the model or (if auto) based on give text and whether it wraps over multiple lines
    void autoSetTextDirection(const QString &text, bool isMultipleLines);

    // Color used for the text.  This is model color if it is valid, otherwise the style color
    QColor textColor() const;

    // need define this for there are overload functions in controller
    MLabel *controller;

    MLabelViewSimple *impl;
    QTextOption textOptions;

    QSizeF paddedSize;
    QTimer *highlighterUpdateTimer;
    bool adjustPaintOffset;
};

#endif
