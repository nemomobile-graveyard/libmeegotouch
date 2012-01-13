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
#include "mlocale.h"
#include "mdeviceprofile.h"
#include "morientationchangeevent.h"
#include "morientationtracker.h"
#include "mcomponentdata.h"

static const QString ellipsisString = QString("...");//QChar(0x2026);

static const int M_HIGHLIGHT_PROPERTY       = QTextFormat::UserProperty;
static const int M_HIGHLIGHTER_ID_PROPERTY  = QTextFormat::UserProperty + 1;

namespace {
struct CursorAndFormat {
    int anchorPos;
    int cursorPos;
    QTextCharFormat format;
};
}

MLabelViewRich::MLabelViewRich(MLabelViewPrivate *viewPrivate) :
    MLabelViewSimple(viewPrivate), textDocumentDirty(true), mouseDownCursorPos(-1),
    tileDimension(-1), tileCacheKey(), tiles(), highlightersChanged(false), isElided(false)
{
    textDocument.setDocumentMargin(0);
    tileCacheKey.sprintf("%p", static_cast<void*>(this));

    const QSize resolution = MDeviceProfile::instance()->resolution();
    tileDimension = qMax(resolution.width(), resolution.height());
}


MLabelViewRich::~MLabelViewRich()
{
    cleanupTiles();
}


void MLabelViewRich::drawContents(QPainter *painter, const QSizeF &size)
{
    initTiles();
    updateTilesPosition();

    // There's no way to set document height to QTextDocument. The document height contains
    // only the area which has text written, therefore being 'compact'. This leads to fact that
    // the vertical alignment is not working, because the widget can be bigger in size when compared
    // to the text document. Therefore we need to do the vertical alignment manually in here.
    // Perform manual alignment for bottom alignment.
    QFontMetrics fm(textDocument.defaultFont());

    const MLabelStyle *style = viewPrivate->style();
    pixmapOffset = QPoint(style->paddingLeft(), style->paddingTop() - fm.leading());
    if (textDocument.size().height() < size.height()) {
        if (viewPrivate->textOptions.alignment() & Qt::AlignTop) {
            // when font has leading < 0 we need to move offset up so that plain
            // text label and rich text label have the same alignment
            pixmapOffset.ry() += qMin(fm.leading(), 0);
        } else if (viewPrivate->textOptions.alignment() & Qt::AlignBottom) {
            pixmapOffset.ry() += fm.leading() + size.height() - textDocument.size().height();
        } else if (viewPrivate->textOptions.alignment() & Qt::AlignVCenter) {
            pixmapOffset.ry() += (fm.leading() + size.height() - textDocument.size().height()) / 2.0;
        }
    }

    if (tiles.isEmpty()) {
        // The QPixmapCache is full. Draw the text directly as fallback.
        qreal x;
        const QRectF textBounds = textBoundaries(&x);
        if (!textBounds.isEmpty()) {
            pixmapOffset.rx() -= x;
            painter->translate(pixmapOffset);
            textDocument.drawContents(painter, textBounds);
        }
    } else {
        drawTiles(painter, pixmapOffset);
    }
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

    textDocumentDirty = true;

    if (viewPrivate->controller->sizePolicy().hasHeightForWidth()) {
        textDocument.setPageSize(event->newSize());
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
        textDocument.setPageSize(event->newSize());
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
        //If we have word wrap or eliding, the minimum size is the size of a single letter,
        if (wrap() || viewPrivate->model()->textElide()) {
            QFontMetrics fm(viewPrivate->controller->font());
            //Add on the line spacing if it's positive, since QTextDocument appears to add line
            //spacing on even for the last line.
            return QSizeF(fm.width(QLatin1Char('x')), fm.height() + qMax(0,fm.leading()));
        }
        //If word wrap and eliding are both disabled (the default) then fall through to preferred
        //size case, so that the preferred size == minimum size.
    }

    case Qt::PreferredSize: {
        // For the preferred size no eliding is wanted. Temporary reset the eliding for
        // calculating the preferred size:
        bool restoreEliding = false;
        QString oldContent;
        if (isElided) {
            oldContent = textDocument.toHtml();
            textDocument.setHtml(wrapTextWithSpanTag(viewPrivate->model()->text()));
            const_cast<bool&>(isElided) = false;
            const_cast<MLabelViewRich*>(this)->updateHighlighters();
            restoreEliding = true;
        }

        // resize text document to constraint width,
        // then return its size.  This works correctly
        // even for a constraint width of -1 (unconstrained)
        QSizeF size;
        if (viewPrivate->controller->sizePolicy().hasHeightForWidth() || constraint.width() >= 0) {
            QSizeF oldPageSize = QSizeF(-1,-1);
            //By default, the label policy has height for width, meaning that the layout
            //will pass us the constraint correctly, so we don't need to do anything special.
            oldPageSize = textDocument.pageSize();
            textDocument.setTextWidth(constraint.width());

            size = textDocument.size();

            int preferredLineCount = viewPrivate->model()->preferredLineCount();
            int preferredLineCountBehavior = viewPrivate->model()->preferredLineCountBehavior();
            if (preferredLineCount < 0) {
                preferredLineCount = viewPrivate->style()->preferredLineCount();
                preferredLineCountBehavior = viewPrivate->stringToLineCountBehavior(viewPrivate->style()->preferredLineCountBehavior());
            }

            if (preferredLineCount == 0) {
                size.setHeight(0);
            } else if (preferredLineCount > 0) {
                QTextCursor cursor = QTextCursor(&textDocument);

                int cursorLine = 0;
                while (++cursorLine < preferredLineCount && cursor.movePosition(QTextCursor::Down)) {}
                QFontMetrics fm(viewPrivate->controller->font());
                if (cursorLine == preferredLineCount) {
                    //We have now placed the cursor on the last visible line.  Find the bottom of this line
                    QTextLayout *layout = cursor.block().layout();
                    QTextLine line = layout->lineForTextPosition(cursor.positionInBlock());
                    size.setHeight(line.position().y() + line.height() + layout->position().y());
                    //If there are lines below this one, we will have to elide, to include the
                    //eliding symbol as part of the preferred width
                    if (constraint.width() == -1 && cursor.movePosition(QTextCursor::Down))
                        size.setWidth( qMax( size.width(), line.width() + fm.boundingRect(ellipsisString).right() + 1 + layout->position().x()));
                } else if (preferredLineCountBehavior == MLabel::LineCountSetsPreferredHeight) {
                    /* Our text has fewer lines than the preferred line count, so if we want to pad
                     * out the preferred size, add on the height of sufficient empty lines.
                     * Note that this should be the same formula as that used in mlabelview_simple.cpp
                     * Note also that we add on leading() (part of lineSpacing) even for the last
                     * line, because that's what QTextDocument does.*/
                    size.rheight() += fm.lineSpacing() * (preferredLineCount - cursorLine);
                }
            }

            textDocument.setPageSize(oldPageSize);
        } else {
            //If the user has manually disabled the sizepolicy heightForWidth and there is
            //no constraint, then we need to fall back to previous behavior of using the current
            //idealWidth as the constraint width
            size = QSizeF(textDocument.idealWidth(), textDocument.size().height());
        }

        if (restoreEliding) {
            textDocument.setHtml(oldContent);
            const_cast<bool&>(isElided) = true;
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
            textDocument.setPageSize(viewPrivate->boundingRect().size());
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
            needUpdate = true;
            textDocumentDirty = true;
        } else if (member == MLabelModel::UseModelFont || member == MLabelModel::Font) {
            textDocument.setPageSize(viewPrivate->boundingRect().size());
            needUpdate = true;
            textDocumentDirty = true;
        } else if (member == MLabelModel::Highlighters) {
            highlightersChanged = true;
        } else if (member == MLabelModel::PreferredLineCount) {
            needUpdate = true;
            textDocumentDirty = true;
        } else if (member == MLabelModel::PreferredLineCountBehavior) {
            needUpdate = true;
            textDocumentDirty = true;
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
    int cursorPos = cursorPosition(event->pos());
    QTextCharFormat format = charFormat(cursorPos);
    if (format.isValid() && format.isAnchor()) {
        //handle the press only if it is happening to standard anchor
        //or if our own highlighter does not want to ignore it
        MLabelHighlighter* h = extractHighlighter(format);
        if (!(h && h->isIgnoringClickEvents())) {
            event->accept();
            viewPrivate->style()->pressFeedback().play();
            mouseDownCursorPos = cursorPos;
            triggerHighlightingUpdate();
        }
    }
}

void MLabelViewRich::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    QTextCharFormat format = charFormat(event->pos());
    if (format.isValid() && format.isAnchor()) {
        //highlighted anchor is released
        MLabelHighlighter* h = extractHighlighter(format);
        if (h && !h->isIgnoringClickEvents()) {
            h->click(format.anchorHref());
            viewPrivate->style()->releaseFeedback().play();
        }
        //application defined anchor is released
        else if (!h) {
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
    // the gesture was inside the highlightable text.
    event->ignore(gesture);

    QPointF gesturePos = viewPrivate->controller->mapFromScene(gesture->position());
    gesturePos -= QPointF(viewPrivate->style()->marginLeft(), viewPrivate->style()->marginTop());
    QTextCharFormat format = charFormat(gesturePos);
    if (format.isValid()) {
        MLabelHighlighter* h = extractHighlighter(format);
        if (h && !h->isIgnoringLongPressEvents()) {
            event->accept(gesture);
            if(gesture->state() == Qt::GestureFinished ) {
                h->longPress(format.anchorHref());
                viewPrivate->style()->pressFeedback().play();
                mouseDownCursorPos = -1;
                triggerHighlightingUpdate();
            }
        }
        // TODO: else if (format.isAnchor()) { model->emitLinkLongPress(format.anchorHref()); }
    }
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
    if (!isElided && shouldElide()) {
        isElided = true;
        textDocument.setPageSize(viewPrivate->boundingRect().size());
        QFontMetrics fm(textDocument.defaultFont());
        QSizeF ellipsisSize = fm.boundingRect(ellipsisString).size();
        if (viewPrivate->boundingRect().width() < ellipsisSize.width() || viewPrivate->boundingRect().height() < ellipsisSize.height()) {
            //There's not even room for the ellipsis
            textDocument.clear();
            return;
        }

        // The ellipsis should start from the last visible line, minus the ellipsis's width and height.  We
        // don't have to be completely accurate in guessing where the ellipsis should go, but we
        // need to only ever overestimate the charPositionForEllipsis value, never underestimate it.
        qreal ellipsisY = qMin(viewPrivate->boundingRect().height(),
                               textDocument.documentLayout()->documentSize().height()) - ellipsisSize.height() + 1;
        if (ellipsisY < 0) {
            // If the document does not contain any text but e.g. only images, the height of the ellipsis might
            // be larger than the height of the document
            ellipsisY = 0;
        }

        Qt::LayoutDirection textDirection = viewPrivate->model()->textDirection();
        if (textDirection == Qt::LayoutDirectionAuto) {
            textDirection = MLocale::directionForText(textDocument.toPlainText());
        }
        const bool rightToLeftDirection = (textDirection == Qt::RightToLeft);
        const QPointF positionForEllipsis = rightToLeftDirection
                                            ? QPointF(ellipsisSize.width() + 1, ellipsisY)
                                            : QPointF(viewPrivate->boundingRect().width() - ellipsisSize.width() + 1, ellipsisY);
        int charPositionForEllipsis = textDocument.documentLayout()->hitTest(positionForEllipsis, Qt::FuzzyHit);

        // Now we have a pretty good guess where the ellipsis should be.  In practise this is almost
        // always completely accurate, but to be doubly sure, delete any letters as necessary until
        // it's the right size.  Since we have to create an edit block anyway, this shouldn't be
        // expensive to do, as long as we only have to delete one or two characters.  Note that we
        // have to keep resetting the page size to force an immediate relayout so that we get a
        // correct documentSize().  Maybe there is a better way?
        QTextCursor cursor(&textDocument);
        cursor.beginEditBlock();
        cursor.movePosition(QTextCursor::End);
        cursor.setPosition(charPositionForEllipsis, QTextCursor::KeepAnchor);
        //Insert the ellipsis string, making sure we clear any existing formatting, leaving only the
        //default font
        QTextCharFormat format;
        format.setFont(textDocument.defaultFont());
        format.setForeground(viewPrivate->textColor());
        cursor.insertText(ellipsisString, format);
        cursor.movePosition(rightToLeftDirection ? QTextCursor::Right : QTextCursor::Left,
                            QTextCursor::MoveAnchor,
                            ellipsisString.size());
        textDocument.setPageSize(viewPrivate->boundingRect().size());

        QTextLayout *layout = cursor.block().layout();
        QTextLine line = layout->lineForTextPosition(cursor.positionInBlock());
        Q_UNUSED(line);
        while (shouldElide()) {
            if (cursor.atStart()) {
                textDocument.clear();
                break;
            }
            int position = cursor.position();
            cursor.deletePreviousChar();
            if (cursor.position() >= position) {
                //This shouldn't happen, but maybe it can with tables, or divs etc.  Best to make sure otherwise we could hang here in an infinite loop
                break;
            }
            textDocument.setPageSize(viewPrivate->boundingRect().size());
        }
        cursor.endEditBlock();
    }
}

bool MLabelViewRich::shouldElide() const
{
    return viewPrivate->model()->textElide()
           && (textDocument.size().width() > viewPrivate->boundingRect().size().width() || textDocument.size().height() > viewPrivate->boundingRect().size().height());
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
    QList<CursorAndFormat> neededFormatChanges;

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
                    // the color did change, so apply the format to the fragment - this has to happen
                    // once iteration is complete as QTextBlock just provides a read-only API
                    // this is quite time consuming, which is the reason for the comparison of old vs. new
                    CursorAndFormat cf;
                    cf.anchorPos = frag.position();
                    cf.cursorPos = frag.position() + frag.length();
                    cf.format = format;
                    neededFormatChanges.push_back(cf);
                }
            }
        }
    }

    QTextCursor cursor(&textDocument);
    foreach (const CursorAndFormat& cf, neededFormatChanges) {
        cursor.setPosition(cf.anchorPos);
        cursor.setPosition(cf.cursorPos, QTextCursor::KeepAnchor);
        cursor.setCharFormat(cf.format);
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

QString MLabelViewRich::renderedText() const
{
    const_cast<MLabelViewRich*>(this)->initTiles();
    return textDocument.toPlainText();
}

bool MLabelViewRich::tileInformation(int index, QPixmap &pixmap, int &y) const
{
    if (!tiles.isEmpty()) {
        if (index >= tiles.count()) {
            pixmap = QPixmap();
            y = 0;
            return true;
        }

        const Tile &tile = tiles.at(index);
        if (QPixmapCache::find(tile.pixmapCacheKey, &pixmap)) {
            y = tile.y;
            return true;
        }
    }

    return false;
}

MLabelHighlighter* MLabelViewRich::extractHighlighter(const QTextCharFormat& format)
{
    if (format.boolProperty(M_HIGHLIGHT_PROPERTY)) {
        int idx = format.intProperty(M_HIGHLIGHTER_ID_PROPERTY);
        if (idx < viewPrivate->model()->highlighters().size() )
            return viewPrivate->model()->highlighters()[idx];
    }
    return NULL;
}

QTextCharFormat MLabelViewRich::charFormat(const QPointF& pos)
{
    return charFormat(cursorPosition(pos));
}

QTextCharFormat MLabelViewRich::charFormat(int cursorPos)
{
    if (cursorPos >= 0) {
        QTextCursor cursor(&textDocument);
        cursor.setPosition(cursorPos);
        cursor.movePosition(QTextCursor::NextCharacter, QTextCursor::KeepAnchor);
        return cursor.charFormat();
    }
    return QTextCharFormat();
}

int MLabelViewRich::cursorPosition(const QPointF& pos)
{
    return textDocument.documentLayout()->hitTest(pos - pixmapOffset, Qt::ExactHit);
}

void MLabelViewRich::initTiles()
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

    int columns = 2;
    int rows = 2;
    QSize tileSize(tileDimension, tileDimension);

    const MLabelStyle *style = viewPrivate->style();

    const int requiredWidth = qMin(viewPrivate->controller->size().width() + style->reactiveMarginLeft() + style->reactiveMarginRight(),
                                   textDocument.size().width() + leftMargin() + rightMargin());
    if (requiredWidth <= tileSize.width()) {
        tileSize.setWidth(requiredWidth);
        columns = 1;
    }

    const int requiredHeight = qMin(viewPrivate->controller->size().height() + style->reactiveMarginTop() + style->reactiveMarginBottom(),
                                    textDocument.size().height() + topMargin() + bottomMargin());
    if (requiredHeight <= tileSize.height()) {
        tileSize.setHeight(requiredHeight);
        rows = 1;
    }

    createTiles(columns, rows, tileSize);
}

void MLabelViewRich::createTiles(int columns, int rows, const QSize &size)
{
    if (size.isEmpty()) {
        return;
    }

    Q_ASSERT(tiles.isEmpty());
    int index = 0;
    int y = -topMargin();
    for (int row = 0; row < rows; ++row) {
        int x = -leftMargin();
        for (int column = 0; column < columns; ++column) {
            Tile tile;
            tiles.append(tile);

            tiles[index].x = x;
            tiles[index].y = y;
            tiles[index].pixmapCacheKey = tileCacheKey + QString::number(index);
            tiles[index].size = size;

            if (!updateTilePixmap(tiles[index])) {
                cleanupTiles();
                return;
            }

            x += size.width();
            ++index;
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

void MLabelViewRich::drawTiles(QPainter *painter, const QPointF &pos)
{
    foreach (const Tile &tile, tiles) {
        QPixmap pixmap;
        if ( !QPixmapCache::find(tile.pixmapCacheKey, pixmap) ) {
            return;
        }

        const QPointF tileOffset(pos.x() + tile.x, pos.y() + tile.y);
        const int minYOffset = -topMargin();
        const MLabelStyle *style = viewPrivate->style();
        const QSizeF labelSize = viewPrivate->controller->size() + QSizeF(0,style->reactiveMarginBottom() + style->reactiveMarginTop());
        const bool clip = -minYOffset + tileOffset.y() + pixmap.height() > labelSize.height()
                          || tileOffset.y() < minYOffset;
        if (clip) {
            painter->save();
            const QRectF clipRect(QPointF(-leftMargin(), minYOffset), labelSize);
            painter->setClipRect(clipRect, Qt::IntersectClip);
            painter->drawPixmap(tileOffset, pixmap);
            painter->restore();
        } else {
            painter->drawPixmap(tileOffset, pixmap);
        }
    }
}

void MLabelViewRich::updateTilesPosition()
{
    const int tilesCount = tiles.count();
    if (tilesCount <= 1) {
        // If only one tile is used, the whole text fits into the tile and no change of the
        // position is required.
        return;
    }

    const QPointF relLabelPos = viewPrivate->controller->pos();
    const QPointF absLabelPos = viewPrivate->mapToRoot(relLabelPos);

    const qreal labelPosXDiff = relLabelPos.x() - absLabelPos.x();
    const qreal labelPosYDiff = relLabelPos.y() - absLabelPos.y();
    const int diff = tileDimension * 2;

    const qreal xMin = labelPosXDiff - tileDimension;
    const qreal xMax = labelPosXDiff + tileDimension;
    const qreal yMin = labelPosYDiff - tileDimension;
    const qreal yMax = labelPosYDiff + tileDimension;

    for (int i = 0; i < tilesCount; ++i) {

        Tile& tile = tiles[i];
        bool update = false;

        while (tile.x < xMin) {
            tile.x += diff;
            update = true;
        }

        while (tile.x > xMax) {
            tile.x -= diff;
            update = true;
        }

        while (tile.y < yMin) {
            tile.y += diff;
            update = true;
        }

        while (tile.y > yMax) {
            tile.y -= diff;
            update = true;
        }

        if (update) {
            updateTilePixmap(tile);
        }
    }
}

int MLabelViewRich::topMargin() const
{
    const MLabelStyle *style = viewPrivate->style();
    return style->paddingTop() + style->marginTop() + style->reactiveMarginTop();
}

int MLabelViewRich::bottomMargin() const
{
    const MLabelStyle *style = viewPrivate->style();
    return style->paddingBottom() + style->marginBottom() + style->reactiveMarginBottom();
}

int MLabelViewRich::leftMargin() const
{
    const MLabelStyle *style = viewPrivate->style();
    return style->paddingLeft() + style->marginLeft() + style->reactiveMarginLeft();
}

int MLabelViewRich::rightMargin() const
{
    const MLabelStyle *style = viewPrivate->style();
    return style->paddingRight() + style->marginRight() + style->reactiveMarginRight();
}

bool MLabelViewRich::updateTilePixmap(const Tile &tile)
{
    qreal x;
    QRectF textBounds = textBoundaries(&x);
    const QRectF tileBounds(tile.x, tile.y, tile.size.width(), tile.size.height());
    textBounds = textBounds.intersected(tileBounds);
    if (textBounds.isEmpty()) {
        QPixmapCache::insert(tile.pixmapCacheKey, QPixmap());
    } else {
        QPixmap pixmap(tile.size);
        pixmap.fill(Qt::transparent);

        QPainter painter(&pixmap);
        painter.translate(-tile.x + x, -tile.y);
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

QRectF MLabelViewRich::textBoundaries(qreal *x) const
{
    QRectF bounds = QRectF(QPointF(0, 0), textDocument.size()).intersected(viewPrivate->boundingRect());

    // Include horizontal padding into the pixmap to make
    // the text wrap correctly, the vertical padding is added
    // when drawing the pixmap into the screen
    bounds.adjust(0, 0, viewPrivate->style()->paddingLeft(), 0);
    // Include the paddings, margins and reactive margins, so that we can paint in
    // them if necessary
    bounds.adjust(-leftMargin(), -topMargin(), rightMargin(), bottomMargin());
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
    const QColor anchorColor(viewPrivate->style()->highlightColor());
    return QString::fromLatin1("* { color: %1; } a {color: %2;}").arg(viewPrivate->textColor().name()).arg(anchorColor.name());
}

void MLabelViewRich::triggerHighlightingUpdate()
{
    // Try to update the highlighting after at least one frame
    const int maxFps = 60;
    viewPrivate->requestHighlighterUpdate(1000 / maxFps);
}
