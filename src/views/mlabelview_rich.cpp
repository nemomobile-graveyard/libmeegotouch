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

#include "mlabelview.h"
#include "mlabelview_p.h"
#include "mlabelmodel.h"
#include "mfeedback.h"
#include "mscenemanager.h"

#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QFontMetrics>
#include <QPixmapCache>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneResizeEvent>
#include <QGestureEvent>
#include <QTapAndHoldGesture>
#include <QTextBlock>
#include "mviewcreator.h"
#include "mlabel.h"
#include "mlabel_p.h"
#include "mlabelhighlighter.h"
#include "mdeviceprofile.h"
#include "morientationchangeevent.h"
#include "morientationtracker.h"
#include "mcomponentdata.h"

static const QString unicodeEllipsisString = QString("...");//QChar(0x2026);

static const int M_HIGHLIGHT_PROPERTY       = QTextFormat::UserProperty;
static const int M_HIGHLIGHTER_ID_PROPERTY  = QTextFormat::UserProperty + 1;

MLabelViewRich::MLabelViewRich(MLabelViewPrivate *viewPrivate) :
    MLabelViewSimple(viewPrivate), textDocumentDirty(true), mouseDownCursorPos(-1),
    tileHeight(-1), tileCacheKey(), tileOrientation(M::Portrait), tiles(), highlightersChanged(false)
    ,isElided(false)
{
    textDocument.setDocumentMargin(0);
    tileCacheKey.sprintf("%p", static_cast<void*>(this));
}


MLabelViewRich::~MLabelViewRich()
{
    cleanupTiles();
}


void MLabelViewRich::drawContents(QPainter *painter, const QSizeF &size)
{
    // Calculate the height of the tile, which represents the screen height
    // in the landscape mode and the screen width in the portrait mode.
    if (tileHeight < 0) {
        const QSize resolution = MDeviceProfile::instance()->resolution();
        tileHeight = (tileOrientation == M::Landscape) ? resolution.height() : resolution.width();
        dirty = true;
    }

    initTiles(QSize(size.width(), tileHeight));
    updateTilesPosition();

    // There's no way to set document height to QTextDocument. The document height contains
    // only the area which has text written, therefore being 'compact'. This leads to fact that
    // the vertical alignment is not working, because the widget can be bigger in size when compared
    // to the text document. Therefore we need to do the vertical alignment manually in here.
    // Perform manual alignment for bottom alignment.
    pixmapOffset = QPoint(0, viewPrivate->style()->paddingTop());
    if (viewPrivate->textOptions.alignment() & Qt::AlignBottom) {
        pixmapOffset.setY(viewPrivate->style()->paddingTop() + size.height() - textDocument.size().height());
    } else if (viewPrivate->textOptions.alignment() & Qt::AlignVCenter) {
        pixmapOffset.setY(viewPrivate->style()->paddingTop() + ((size.height() / 2) - (textDocument.size().height() / 2)));
    }

    if (tiles.isEmpty()) {
        // The QPixmapCache is full. Draw the text directly as fallback.
        qreal x;
        const QRectF textBounds = textBoundaries(&x);
        pixmapOffset.rx() -= x;
        painter->translate(pixmapOffset);
        textDocument.drawContents(painter, textBounds);
    } else {
        drawTiles(painter, pixmapOffset, size);
    }
}

/**
  * Returns true if label should be elided.
  */
bool MLabelViewRich::shouldElide() const
{
    static const int MAXIMUM_NON_ELIDING_TEXT_SIZE = 4;

    return viewPrivate->model()->textElide() && (textDocument.size().width() > viewPrivate->boundingRect().size().width()) && (textDocument.characterCount() > MAXIMUM_NON_ELIDING_TEXT_SIZE);

}

void MLabelViewRich::ensureDocumentIsReady()
{
    if (textDocumentDirty) {
        // QTextDocument doesn't update layout when text options
        // are set.
        textDocumentDirty = false;
        textDocument.setDefaultTextOption(viewPrivate->textOptions);
        textDocument.setUndoRedoEnabled(true);
        textDocument.setDefaultFont(viewPrivate->controller->font());
        // default text color can be specified only via stylesheet. However
        // text should be enclosed with tags: <span>text</span> to let CSS
        // engine apply color
        textDocument.setDefaultStyleSheet(defaultStyleSheet());

        // To force it relayout text, it should be set again
        QString t = viewPrivate->model()->text();
        textDocument.setHtml(wrapTextWithSpanTag(t));
        isElided = false;
        updateHighlighters();
        //textDocument.setHtml(wrapTextWithSpanTag(viewPrivate->model()->text()));
    }
}

bool MLabelViewRich::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    // We support two different modes of operation.  If heightForWidth is set, then the layout
    // that we are in will set our size correctly, and nothing more is needed.
    //
    // However users can optionally disable heightForWidth to get a slightly different
    // behavior, getting a tight bounding box.

    if (viewPrivate->controller->sizePolicy().hasHeightForWidth()) {
        //We just need to resize the document, and nothing more needed
        textDocument.setTextWidth(event->newSize().width());
        return false; //No invalidate ever needed
    } else {
        // Without height for width, we have to invalidate when we change size.

        // 1st phase, when Qt calls sizeHint, view will return approximate values for
        // minimum and preferred size. When resizeEvent comes, layout already knows
        // sizes of components, and here comes
        // 2nd phase, when we identify widget's height, based on width. Our height will
        // change and we don't want to occupy more space then need, so we have to call
        // updateGeometry, to tell layout to update sizeHint cache. This function
        // return true if such update is needed.
        // forward resize event to text document
        // if height is changed
        QSizeF oldSize(textDocument.size());
        textDocument.setTextWidth(event->newSize().width());
        QSizeF newSize(textDocument.size());
        if (newSize.height() != oldSize.height())
            return true;
        else
            return false;
    }
}

QSizeF MLabelViewRich::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch (which) {
    case Qt::MinimumSize: {
        QFontMetrics fm(viewPrivate->controller->font());
        //Resize text document to maximum width to find the height of one line
        //This follows the QLabel implementation
        qreal oldWidth = textDocument.textWidth();
        textDocument.setTextWidth(QWIDGETSIZE_MAX);
        qreal height = textDocument.size().height();
        textDocument.setTextWidth(oldWidth);
        return QSizeF(fm.width('x'), height);
    }

    case Qt::PreferredSize: {
        //remove existing eliding if there
        if(isElided) {
            textDocument.setHtml(wrapTextWithSpanTag(viewPrivate->model()->text()));
            const_cast<bool&>(isElided) = false;
            const_cast<MLabelViewRich*>(this)->updateHighlighters();
        }

        // resize text document to constraint width,
        // then return its size.  This works correctly
        // even for a constraint width of -1 (unconstrained)
        QSizeF size;
        if (viewPrivate->controller->sizePolicy().hasHeightForWidth() || constraint.width() >= 0) {
            //By default, the label policy has height for width, meaning that the layout
            //will pass us the constraint correctly, so we don't need to do anything special.
            qreal oldWidth = textDocument.textWidth();
            textDocument.setTextWidth(constraint.width());
            size = textDocument.size();
            textDocument.setTextWidth(oldWidth);
        } else {
            //If the user has manually disabled the sizepolicy heightForWidth and there is
            //no constraint, then we need to fall back to previous behavior of using the current
            //idealWidth as the constraint width
            size = QSizeF(textDocument.idealWidth(), textDocument.size().height());
        }

        return size;
    }

    case Qt::MaximumSize:
        return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);

    default:
        qWarning("MLabel::sizeHint() don't know how to handle the value of 'which' ");
    }

    return QSizeF(0, 0);
}

void MLabelViewRich::setupModel()
{
    const MLabelModel *model = viewPrivate->model();

    if (const MSceneManager *sceneManager = viewPrivate->controller->sceneManager()) {
        tileOrientation = sceneManager->orientation();
    } else if (const MWindow *activeWindow = MComponentData::activeWindow()) {
        tileOrientation = activeWindow->orientation();
    }

    if (model->wordWrap()) {
        viewPrivate->textOptions.setWrapMode(model->wrapMode());
    } else {
        //NOTE!!: ManualWrap used instead of NoWrap. NoWrap works incorrectly in Qt 4.6
        viewPrivate->textOptions.setWrapMode(QTextOption::ManualWrap);
    }
    viewPrivate->textOptions.setTextDirection(model->textDirection());
    viewPrivate->textOptions.setAlignment(model->alignment());

    // We have to update rich text document now because next call
    // can be sizeHint and at that time will be too late to do it
    //textDocument.setHtml(model->text());
    textDocumentDirty = true;
    ensureDocumentIsReady();
}

bool MLabelViewRich::updateData(const QList<const char *>& modifications)
{
    //mark as true if sizehint needs updating
    bool needUpdate = false;

    const MLabelModel *model = viewPrivate->model();
    const char *member = NULL;
    foreach(member, modifications) {
        if (member == MLabelModel::Text) {
            //textDocument.setHtml(model->text());

            // Document text width must be enlarged just like resizeEvent()
            // does with labels that initially have richtext content.
            textDocument.setTextWidth(viewPrivate->boundingRect().width());
            needUpdate = true;
            textDocumentDirty = true;
        } else if (member == MLabelModel::Color) {
            needUpdate = true;
            textDocumentDirty = true;
        } else if(member == MLabelModel::WrapMode) {
            if (model->wordWrap()) {
                if (model->wrapMode() == QTextOption::NoWrap) {
                    //NOTE!!: ManualWrap used instead of NoWrap. NoWrap works incorrectly in Qt 4.6
                    viewPrivate->textOptions.setWrapMode(QTextOption::ManualWrap);
                } else {
                    viewPrivate->textOptions.setWrapMode(model->wrapMode());
                }
                textDocumentDirty = true;
            }
            needUpdate = true;
        } else if (member == MLabelModel::WordWrap) {
            if (model->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(model->wrapMode());
            } else {
                //NOTE!!: ManualWrap used instead of NoWrap. NoWrap works incorrectly in Qt 4.6
                viewPrivate->textOptions.setWrapMode(QTextOption::ManualWrap);
            }
            textDocumentDirty = true;
            needUpdate = true;
        } else if (member == MLabelModel::TextDirection) {
            viewPrivate->textOptions.setTextDirection(model->textDirection());
            textDocumentDirty = true;
        } else if (member == MLabelModel::Alignment) {
            viewPrivate->textOptions.setAlignment(model->alignment());
            textDocumentDirty = true;
        } else if (member == MLabelModel::TextElide) {
            textDocumentDirty = true;
        } else if (member == MLabelModel::UseModelFont || member == MLabelModel::Font) {
            textDocument.setTextWidth(viewPrivate->boundingRect().width());
            needUpdate = true;
            textDocumentDirty = true;
        } else if (member == MLabelModel::Highlighters) {
            highlightersChanged = true;
        }
    }

    // We have to update rich text document now because next call
    // can be sizeHint and at that time will be too late to do it
    ensureDocumentIsReady();
    return needUpdate;
}

bool MLabelViewRich::isRich()
{
    return true;
}

void MLabelViewRich::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore();
    int cursorPos = textDocument.documentLayout()->hitTest(event->pos() - pixmapOffset, Qt::ExactHit);
    if (cursorPos >= 0) {
        QTextCursor cursor(&textDocument);
        cursor.setPosition(cursorPos+1);
        QTextCharFormat format = cursor.charFormat();
        mouseDownCursorPos = cursorPos;
        if (format.isAnchor()) {
            triggerHighlightingUpdate();
            event->accept();
            viewPrivate->style()->pressFeedback().play();
        }
    }
}

void MLabelViewRich::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    int cursorPos = textDocument.documentLayout()->hitTest(event->pos() - pixmapOffset, Qt::ExactHit);
    if (cursorPos >= 0) {
        QTextCursor cursor(&textDocument);
        cursor.setPosition(cursorPos+1);
        QTextCharFormat format = cursor.charFormat();
        //highlighted anchor is released
        if (format.boolProperty(M_HIGHLIGHT_PROPERTY)) {
            int idx = cursor.charFormat().intProperty(M_HIGHLIGHTER_ID_PROPERTY);
            if (idx < viewPrivate->model()->highlighters().size())
                viewPrivate->model()->highlighters()[idx]->click(format.anchorHref());

            viewPrivate->style()->releaseFeedback().play();
        }
        //application defined anchor is released
        else if (format.isAnchor()) {
            viewPrivate->model()->emitLinkActivated(format.anchorHref());
            viewPrivate->style()->releaseFeedback().play();
        }
    }
    mouseDownCursorPos = -1;
    triggerHighlightingUpdate();
}

void MLabelViewRich::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    if (mouseDownCursorPos > 0) {
        mouseDownCursorPos = -1;
        viewPrivate->style()->cancelFeedback().play();
        triggerHighlightingUpdate();
    }
}

void MLabelViewRich::longPressEvent(QGestureEvent *event, QTapAndHoldGesture* gesture)
{
    // We ignore the gesture by default and only accept it later if the hot spot of
    // the gesture was insid highlightable text.
    event->ignore(gesture);

    QPointF gesturePos = viewPrivate->controller->mapFromScene(gesture->position() - QPointF(viewPrivate->style()->marginLeft(), viewPrivate->style()->marginTop()));
    int cursorPos = textDocument.documentLayout()->hitTest(gesturePos - pixmapOffset, Qt::ExactHit);
    if (cursorPos >= 0) {
        QTextCursor cursor(&textDocument);
        cursor.setPosition(cursorPos+1);
        QTextCharFormat format = cursor.charFormat();
        if (format.boolProperty(M_HIGHLIGHT_PROPERTY)) {
            event->accept(gesture);
            int idx = cursor.charFormat().intProperty(M_HIGHLIGHTER_ID_PROPERTY);
            if (idx < viewPrivate->model()->highlighters().size() && gesture->state() == Qt::GestureFinished) {
                viewPrivate->model()->highlighters()[idx]->longPress(format.anchorHref());
                viewPrivate->style()->pressFeedback().play();
                mouseDownCursorPos = -1;
                triggerHighlightingUpdate();
            }
        }
        // TODO: else if (format.isAnchor()) { model->emitLinkLongPress(format.anchorHref()); }
    }
}

void MLabelViewRich::orientationChangeEvent(MOrientationChangeEvent *event)
{
    tileOrientation = event->orientation();
    tileHeight = -1;
}

/**
 * Find cursor position of last visible character of document.
 * It starts from the bottom right corner, goes up until finds
 * text by hitTest function.
 *
 * @return cursor position
 */
int MLabelViewRich::cursorPositionOfLastVisibleCharacter()
{
    QPointF bottomRight(viewPrivate->boundingRect().bottomRight());
    // defines how quickly we will scan through area
    QPointF delta(0, 5);
    int cursorPos = -1;
    while (bottomRight.y() > delta.y() && cursorPos == -1) {
        cursorPos = textDocument.documentLayout()->hitTest(bottomRight, Qt::ExactHit);
        bottomRight -= delta;
    }

    return cursorPos;
}

void MLabelViewRich::updateRichTextEliding()
{
    if (shouldElide()) {
        //TODO
        //Reconsider this. How can we efficiently find last FULLY visible character?
        //Hit testing per pixel basis was bad and was not properly working. This
        //implementation is as bad but this works correctly.

        //estimate the last visible character by roughly removing text from label
        int charJump = (textDocument.characterCount() <= 30) ? textDocument.characterCount() / 2 : 20;
        QTextCursor cursor(&textDocument);
        cursor.beginEditBlock();
        while (shouldElide()) {
            cursor.movePosition(QTextCursor::End);
            charJump = (cursor.position() <= charJump) ? (textDocument.characterCount() / 2) : 20;
            cursor.setPosition(cursor.position() - charJump, QTextCursor::KeepAnchor);
            cursor.removeSelectedText();
            textDocument.setTextWidth(viewPrivate->boundingRect().size().width());
        }
        cursor.endEditBlock();
        int charcount = textDocument.characterCount() + charJump;
        textDocument.undo();

        //find the last visible character by removing one character at a time
        //starting from the rough estimation end position
        charJump = 1;
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::End);
        cursor.movePosition(QTextCursor::Left, QTextCursor::KeepAnchor, textDocument.characterCount() - charcount + unicodeEllipsisString.size());
        cursor.insertText(unicodeEllipsisString);
        textDocument.setTextWidth(viewPrivate->boundingRect().size().width());
        while (shouldElide()) {
            cursor.movePosition(QTextCursor::End);
            cursor.setPosition(cursor.position() - charJump - unicodeEllipsisString.size(), QTextCursor::KeepAnchor);
            cursor.insertText(unicodeEllipsisString);
            textDocument.setTextWidth(viewPrivate->boundingRect().size().width());
        }
        cursor.endEditBlock();
        isElided = true;
    }
}

void MLabelViewRich::updateHighlighters()
{
    //TODO: Should the highlight format come from the highlighter object?
    QList<MLabelHighlighter *> list = viewPrivate->model()->highlighters();
    const int listSize = list.size();
    for (int i = 0; i < listSize; ++i) {
        const MLabelHighlighter *highlighter = list[i];
        QTextCursor cursor(&textDocument);
        while (!cursor.isNull() && !cursor.atEnd()) {
            cursor = textDocument.find(highlighter->highlightExpression(), cursor);

            //Regexp matching an empty string doesn’t advance cursor
            if (cursor.selectedText().isNull()) {
                break;
            }

            if (!cursor.isNull()) {
                QString item = cursor.selectedText();
                if (highlighter->validate(item)) {
                    const QTextCharFormat oldFormat = cursor.charFormat();
                    QTextCharFormat format = oldFormat;
                    format.setFontUnderline(true);
                    format.setAnchor(true);
                    format.setAnchorHref(item);
                    format.setProperty(M_HIGHLIGHT_PROPERTY, true);
                    format.setProperty(M_HIGHLIGHTER_ID_PROPERTY, i);
                    format.setForeground(QBrush(viewPrivate->style()->highlightColor()));
                    if (oldFormat != format) {
                        cursor.setCharFormat(format);
                    }
                }
            }
        }
    }
    highlightersChanged = false;
}

void MLabelViewRich::updateHighlighting()
{
    QTextCursor cursor(&textDocument);
    for (QTextBlock block = textDocument.begin(); block != textDocument.end(); block = block.next()) {
        QTextBlock::iterator it = block.begin();
        for(;!(it.atEnd()); ++it) {
            QTextFragment frag = it.fragment();
            QTextCharFormat format = frag.charFormat();
            if(format.isAnchor()) {
                if (mouseDownCursorPos != -1 && frag.contains(mouseDownCursorPos)) {
                    // this fragment is currently "pressed"
                    format.setForeground(QBrush(viewPrivate->style()->activeHighlightColor()));
                } else {
                    // this fragment is currently "released"
                    format.setForeground(QBrush(viewPrivate->style()->highlightColor()));
                }
                if(frag.charFormat() != format) {
                    // the color did change, so apply the format to the fragment
                    // this is quite time consuming, which is the reason for the comparison of old vs. new
                    cursor.setPosition(frag.position());
                    cursor.setPosition(frag.position()+frag.length(), QTextCursor::KeepAnchor);
                    cursor.setCharFormat(format);
                }
            }
        }
    }
}

QString MLabelViewRich::wrapTextWithSpanTag(const QString &text) const
{
    QString spanTag("<span>%1</span>");
    return spanTag.arg(text);
}

void MLabelViewRich::applyStyle()
{
    const QString styleSheet = defaultStyleSheet();
    if (textDocument.defaultStyleSheet() != styleSheet) {
        textDocumentDirty = true;
        textDocument.setDefaultStyleSheet(styleSheet);
    }
}

void MLabelViewRich::initTiles(const QSize &size)
{
    if (dirty || textDocumentDirty || highlightersChanged) {
        cleanupTiles();
        dirty = false;
    }

    if (!tiles.isEmpty()) {
        // The tiles should be cached already, but it might be possible that
        // the cache got invalid in the meantime. In this case just try
        // to update the cache again.
        if (isTilesCacheValid()) {
            return;
        }
        cleanupTiles();
    }

    Q_ASSERT(tiles.isEmpty());

    if (textDocumentDirty) {
        ensureDocumentIsReady();
    } else if (highlightersChanged) {
        QString t = viewPrivate->model()->text();
        textDocument.setHtml(wrapTextWithSpanTag(t));
        isElided = false;
        updateHighlighters();
    }
    updateRichTextEliding();
    updateHighlighting();

    int tileCount = 2;
    QSize tileSize = size;

    const int requiredHeight = textDocument.size().height();
    if (requiredHeight < tileSize.height()) {
        tileSize.setHeight(requiredHeight);
        tileCount = 1;
    }

    createTiles(tileCount, tileSize);
}

void MLabelViewRich::createTiles(int count, const QSize &size)
{
    Q_ASSERT(tiles.isEmpty());

    int y = 0;
    for (int i = 0; i < count; ++i) {
        Tile tile;
        tiles.append(tile);

        tiles[i].y = y;
        tiles[i].pixmapCacheKey = tileCacheKey + QString::number(i);
        tiles[i].size = size;

        if (!updateTilePixmap(tiles[i])) {
            cleanupTiles();
            return;
        }

        y += size.height();
    }
}

void MLabelViewRich::cleanupTiles()
{
    foreach (const Tile &tile, tiles) {
        QPixmapCache::remove(tile.pixmapCacheKey);
    }
    tiles.clear();
}

void MLabelViewRich::drawTiles(QPainter *painter, const QPointF &pos, const QSizeF &size)
{
    foreach (const Tile &tile, tiles) {
        if (tile.y >= 0.0) {
            QPixmap pixmap;
            if ( !QPixmapCache::find(tile.pixmapCacheKey, pixmap) ) {
                return;
            }

            const QPointF tileOffset(pos.x(), pos.y() + tile.y);
            const bool clip = tileOffset.y() + pixmap.height() > size.height() ||
                              tileOffset.y() + pixmap.height() <= 0.0;
            if (clip) {
                painter->save();
                const QRectF clipRect(QPointF(0, 0), size);
                painter->setClipRect(clipRect, Qt::IntersectClip);
                painter->drawPixmap(tileOffset, pixmap);
                painter->restore();
            } else {
                painter->drawPixmap(tileOffset, pixmap);
            }
        }
    }
}

void MLabelViewRich::updateTilesPosition()
{
    if (tiles.count() <= 1) {
        // If only one tile is used, the whole text fits into the tile and no change of the
        // position is required.
        return;
    }

    Q_ASSERT(tiles.count() == 2);
    Tile *top = topTile();
    Tile *bottom = bottomTile();

    // Update the tile pixmaps and position, if they got invalid because of a changed scene position
    const QPointF scenePos = viewPrivate->controller->scenePos();
    const int sceneY = (tileOrientation == M::Landscape) ? scenePos.y() : scenePos.x();

    // Check if none of the tiles are visible
    if (sceneY + bottom->y + tileHeight < 0.0) {
        // Put them where they're visible
        top->y = qAbs(sceneY);
        bottom->y = top->y + tileHeight;
        updateTilePixmap(*top);
        updateTilePixmap(*bottom);
    }
    else if (sceneY + top->y + tileHeight < 0.0) {
        // The top tile got invisible, use it as bottom tile for the next iteration
        top->y = bottom->y + tileHeight;
        updateTilePixmap(*top);
        viewPrivate->controller->update();
    } else if (sceneY + top->y > 0.0) {
        // The bottom tile got invisible, use it as top tile for the next iteration
        bottom->y = top->y - tileHeight;
        if (bottom->y >= 0.0) {
            updateTilePixmap(*bottom);
        }
        viewPrivate->controller->update();
    }
}

bool MLabelViewRich::updateTilePixmap(const Tile &tile)
{
    qreal x;
    QRectF textBounds = textBoundaries(&x);
    textBounds = textBounds.intersected(QRectF(x, tile.y, tile.size.width(), tile.size.height()));
    if (!textBounds.isEmpty()) {
        QPixmap pixmap(tile.size);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.translate(viewPrivate->style()->paddingLeft() - x, -tile.y);
        textDocument.drawContents(&painter, textBounds);

        return QPixmapCache::insert(tile.pixmapCacheKey, pixmap);
    }
    return true;
}

bool MLabelViewRich::isTilesCacheValid() const
{
    QPixmap pixmap;
    foreach (const Tile &tile, tiles) {
        if (!QPixmapCache::find(tile.pixmapCacheKey, pixmap)) {
            return false;
        }
    }
    return true;
}

MLabelViewRich::Tile* MLabelViewRich::topTile()
{
    Q_ASSERT(tiles.count() == 2);
    return (tiles[0].y <= tiles[1].y) ? &tiles[0] : &tiles[1];
}

MLabelViewRich::Tile* MLabelViewRich::bottomTile()
{
    Q_ASSERT(tiles.count() == 2);
    return (tiles[0].y <= tiles[1].y) ? &tiles[1] : &tiles[0];
}

QRectF MLabelViewRich::textBoundaries(qreal *x) const
{
    QRectF bounds = QRectF(QPointF(0, 0), textDocument.size()).intersected(viewPrivate->boundingRect());

    // Include horizontal padding into the pixmap to make
    // the text wrap correctly, the vertical padding is added
    // when drawing the pixmap into the screen
    bounds.adjust(0, 0, viewPrivate->style()->paddingLeft(), 0);

    Q_ASSERT(x);
    *x = 0.0;

    if (viewPrivate->controller->layoutDirection() == Qt::RightToLeft) {
        const qreal diff = textDocument.size().width() - viewPrivate->boundingRect().width();
        if (diff > 0.0) {
            *x = diff;
            bounds.moveLeft(diff);
        }
    }

    return bounds;
}

QString MLabelViewRich::defaultStyleSheet() const
{
    const QColor textColor(viewPrivate->model()->color().isValid() ? viewPrivate->model()->color() : viewPrivate->style()->color());
    const QColor anchorColor(viewPrivate->style()->highlightColor());
    return QString::fromLatin1("* { color: %1; } a {color: %2;}").arg(textColor.name()).arg(anchorColor.name());
}

void MLabelViewRich::triggerHighlightingUpdate()
{
    // Try to update the highlighting after at least one frame
    const int maxFps = 60;
    viewPrivate->requestHighlighterUpdate(1000 / maxFps);
}
