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

#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QFontMetrics>
#include <QPixmapCache>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneResizeEvent>
#include "mviewcreator.h"
#include "mlabel.h"
#include "mlabel_p.h"
#include "mlabelhighlighter.h"
#include "mdebug.h"

static const QString unicodeEllipsisString = QString("...");//QChar(0x2026);

static const int M_HIGHLIGHT_PROPERTY       = QTextFormat::UserProperty;
static const int M_HIGHLIGHTER_ID_PROPERTY  = QTextFormat::UserProperty + 1;

MLabelViewRich::MLabelViewRich(MLabelViewPrivate *viewPrivate) :
    MLabelViewSimple(viewPrivate), textDocumentDirty(true), mouseDownCursorPos(-1)
{
    textDocument.setDocumentMargin(0);
}    


MLabelViewRich::~MLabelViewRich()
{
}


QPixmap MLabelViewRich::generatePixmap()
{
    ensureDocumentIsReady();
    updateRichTextEliding();
    updateHighlighting();

    // Document's rect can be very big if it doesn't fit label's space, however
    // label as well can be bigger then the amount of text. Intersecting 2 rectangles
    // will give us smallest visible area, where content should be painted.
    QRectF paintingRect = QRectF(QPointF(0, 0), textDocument.size()).intersected(viewPrivate->boundingRect());

    //include horizontal padding into the pixmap to make
    //the text wrap correctly, the vertical padding is added
    //when drawing the pixmap into the screen
    const MLabelStyle *style = viewPrivate->style();
    paintingRect.adjust(0, 0, style->paddingLeft(), 0);
    if (paintingRect.isEmpty()) {
        return QPixmap();
    }

    //draw the textdocument into the pixmap
    QPixmap pixmap(paintingRect.size().toSize());
    if (!pixmap.isNull()) {
        pixmap.fill(Qt::transparent);
        QPainter painter(&pixmap);
        painter.translate(style->paddingLeft(), 0);
        textDocument.drawContents(&painter, paintingRect);
    }

    return pixmap;
}

void MLabelViewRich::drawContents(QPainter *painter, const QSizeF &size)
{
    if (!QPixmapCache::find(viewPrivate->cacheKey, pixmap)) {
        pixmap = generatePixmap();
        QPixmapCache::insert(viewPrivate->cacheKey, pixmap);
    }
    if (!pixmap.isNull()) {
        // There's no way to set document height to QTextDocument. The document height contains
        // only the area which has text written, therefore being 'compact'. This leads to fact that
        // the vertical alignment is not working, because the widget can be bigger in size when compared
        // to the text document. Therefore we need to do the vertical alignment manually in here.
        // Perform manual alignment for bottom alignment.
        pixmapOffset = QPoint(0, viewPrivate->style()->paddingTop());
        if (viewPrivate->textOptions.alignment() & Qt::AlignBottom) {
            pixmapOffset.setY(viewPrivate->style()->paddingTop() + size.height() - pixmap.size().height());
        }
        // Perform manual alignment for vertical center alignment.
        else if (viewPrivate->textOptions.alignment() & Qt::AlignVCenter) {
            pixmapOffset.setY(viewPrivate->style()->paddingTop() + ((size.height() / 2) - (pixmap.size().height() / 2)));
        }

        painter->drawPixmap(pixmapOffset, pixmap);
    }
}

/**
  * Returns true if label should be elided.
  */
bool MLabelViewRich::shouldElide() const
{
    static const int MAXIMUM_NON_ELIDING_TEXT_SIZE = 4;
    return viewPrivate->model()->textElide() && ((textDocument.size().width() > viewPrivate->boundingRect().size().width()) || (textDocument.size().height() > viewPrivate->boundingRect().size().height())) && (textDocument.characterCount() > MAXIMUM_NON_ELIDING_TEXT_SIZE);
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
        QColor textColor(viewPrivate->model()->color().isValid() ? viewPrivate->model()->color() : viewPrivate->style()->color());
        QColor anchorColor(viewPrivate->style()->highlightColor());
        QString styleSheet = QString::fromLatin1("* { color: %1; } a {color: %2;}").arg(textColor.name()).arg(anchorColor.name());
        textDocument.setDefaultStyleSheet(styleSheet);

        // To force it relayout text, it should be set again
        QString t = viewPrivate->model()->text();
        textDocument.setHtml(wrapTextWithSpanTag(t));
        //textDocument.setHtml(wrapTextWithSpanTag(viewPrivate->model()->text()));
    }
}

bool MLabelViewRich::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    // There is no way to specify sizeHint for a text without knowing possible width.

    // 1st phase, when QT calls sizeHint, view will return approximate values for
    // minimum and preffered size. When resizeEvent comes, layout already knows
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
        textDocument.undo();

        // resize text document to constraint width,
        // then return its size
        QSizeF size;
        if (constraint.width() > 0) {
            qreal oldWidth = textDocument.textWidth();
            textDocument.setTextWidth(constraint.width());
            size = textDocument.size();
            textDocument.setTextWidth(oldWidth);
        } else {
            size = textDocument.size();
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
    int cursorPos = textDocument.documentLayout()->hitTest(event->pos() - pixmapOffset, Qt::ExactHit);
    if (cursorPos >= 0) {
        QTextCursor cursor(&textDocument);
        cursor.setPosition(cursorPos);
        QTextCharFormat format = cursor.charFormat();
        //highlighted anchor is pressed
        if (format.boolProperty(M_HIGHLIGHT_PROPERTY)) {
            event->accept();
            mouseDownCursorPos = cursorPos;
            QPixmapCache::remove(viewPrivate->cacheKey);
            viewPrivate->controller->update();
            viewPrivate->style()->pressFeedback().play();
        }
        //application defined anchor is pressed
        else if (format.isAnchor()) {
            event->accept();
            viewPrivate->style()->pressFeedback().play();
        }
    }
}

void MLabelViewRich::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    mouseDownCursorPos = -1;
    QPixmapCache::remove(viewPrivate->cacheKey);
    viewPrivate->controller->update();
    
    int cursorPos = textDocument.documentLayout()->hitTest(event->pos() - pixmapOffset, Qt::ExactHit);
    if (cursorPos >= 0) {
        QTextCursor cursor(&textDocument);
        cursor.setPosition(cursorPos);
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
            viewPrivate->style()->releaseFeedback().play();
            viewPrivate->model()->emitLinkActivated(format.anchorHref());
        }
    }
}

void MLabelViewRich::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
    if (mouseDownCursorPos > 0) {
        mouseDownCursorPos = -1;
        QPixmapCache::remove(viewPrivate->cacheKey);
        viewPrivate->controller->update();
        viewPrivate->style()->cancelFeedback().play();
    }
}

void MLabelViewRich::longPressEvent(QGraphicsSceneContextMenuEvent *event)
{
    int cursorPos = textDocument.documentLayout()->hitTest(event->pos() - pixmapOffset, Qt::ExactHit);
    if (cursorPos >= 0) {
        QTextCursor cursor(&textDocument);
        cursor.setPosition(cursorPos);
        QTextCharFormat format = cursor.charFormat();
        if (format.boolProperty(M_HIGHLIGHT_PROPERTY)) {
            event->accept();
            int idx = cursor.charFormat().intProperty(M_HIGHLIGHTER_ID_PROPERTY);
            if (idx < viewPrivate->model()->highlighters().size())
                viewPrivate->model()->highlighters()[idx]->longPress(format.anchorHref());

            viewPrivate->style()->pressFeedback().play();
        }
    }
    mouseDownCursorPos = -1;
    QPixmapCache::remove(viewPrivate->cacheKey);
    viewPrivate->controller->update();
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
    // if we cut several characters last time
    // undo() will put them back
    textDocument.undo();

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
    }
}

void MLabelViewRich::updateHighlighting()
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
                    QTextCharFormat format = cursor.charFormat();
                    format.setFontUnderline(true);
                    format.setAnchor(true);
                    format.setAnchorHref(item);
                    format.setProperty(M_HIGHLIGHT_PROPERTY, true);
                    format.setProperty(M_HIGHLIGHTER_ID_PROPERTY, i);
                    if (mouseDownCursorPos >= cursor.selectionStart() && mouseDownCursorPos <= cursor.selectionEnd()) {
                        format.setForeground(QBrush(viewPrivate->style()->activeHighlightColor()));
                        //format.setBackground(QBrush(viewPrivate->style()->activeHighlightColor()));
                    } else
                        format.setForeground(QBrush(viewPrivate->style()->highlightColor()));
                    //cursor.mergeCharFormat(format);
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
    textDocumentDirty = true;
    ensureDocumentIsReady();
}
