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
#include "mlabel.h"
#include "mviewcreator.h"
#include "melidedtext.h"

#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QTransform>
#include <QFontMetrics>
#include <QPixmapCache>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneResizeEvent>
#include <QGestureEvent>
#include <QTapAndHoldGesture>
namespace
{
    const QChar TextVariantSeparator(0x9c, 0);
    const QChar EllipsisChar(0x2026);

} // namespace 

MLabelViewSimple::MLabelViewSimple(MLabelViewPrivate *viewPrivate) :
    viewPrivate(viewPrivate), preferredSize(-1, -1), textOffset(), paintingRect(), dirty(true), staticText(), clip(false)
{
    stringVariants << "";
    staticText.setPerformanceHint(QStaticText::AggressiveCaching);
    staticText.setTextFormat(Qt::PlainText);
}

MLabelViewSimple::~MLabelViewSimple()
{
}

void MLabelViewSimple::drawContents(QPainter *painter, const QSizeF &size)
{
    Q_UNUSED(size);

    initializeTextProperties();
    if (staticText.text().isEmpty() || paintingRect.isEmpty()) {
        return;
    }

    painter->setPen(pen);
    painter->setFont(font);
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setLayoutDirection(viewPrivate->model()->textDirection());

    if (clip) {
        painter->save();
        painter->setClipRect(paintingRectWithMargins, Qt::IntersectClip);
        painter->drawStaticText(textOffset, staticText);
        painter->restore();
    } else {
        painter->drawStaticText(textOffset, staticText);
    }
}

bool MLabelViewSimple::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    // We support two different modes of operation.  If heightForWidth is set, then the layout
    // that we are in will set our size correctly, and nothing more is needed.
    //
    // However users can optionally disable heightForWidth to get a slightly different
    // behavior, getting a tight bounding box.

    if (viewPrivate->controller->sizePolicy().hasHeightForWidth()) {
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
        QFontMetricsF fm(viewPrivate->controller->font());

        QString text = stringVariants.first();

        QRectF bR = fm.boundingRect(QRectF(QPoint(0, 0), event->newSize()), 
                viewPrivate->textOptions.alignment() | textFlagForWrapMode(),
                text);
        if (bR.height() > fm.height()) {
            preferredSize = QSizeF(bR.width(), bR.height());
            return true;
        } else
            return false;
    }
}
QSizeF MLabelViewSimple::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch (which) {
    case Qt::MinimumSize: {
        //If we have word wrap, eliding, or a width constraint, then the minimum size is the size of a single letter,
        if (wrap() || viewPrivate->model()->textElide() || constraint.width() >= 0 || stringVariants.first().isEmpty()) {
            QFontMetricsF fm(viewPrivate->controller->font());
            //Add on the line spacing if it's positive, since QTextDocument appears to add line
            //spacing on even for the last line.
            return QSizeF(fm.width(QLatin1Char('x')), fm.height() + qMax<qreal>(0,fm.leading()));
        }
        //If word wrap and eliding are both disabled (the default) then use the smallest string as
        //the minimum size
        return sizeForWidth(-1, stringVariants.last(), 1, MLabel::LineCountLimitsPreferredHeight); //Get the size of the smallest variant
    }
    case Qt::PreferredSize: {
        qreal width = constraint.width();
        if (!viewPrivate->controller->sizePolicy().hasHeightForWidth()) {
            if (preferredSize.width() >= 0 && (preferredSize.width() < width || width < 0))
                width = preferredSize.width();
        }
        //Model overrides style
        int preferredLineCount = viewPrivate->model()->preferredLineCount();
        int preferredLineCountBehavior = viewPrivate->model()->preferredLineCountBehavior();
        if (preferredLineCount < 0) {
            preferredLineCount = viewPrivate->style()->preferredLineCount();
            preferredLineCountBehavior = viewPrivate->stringToLineCountBehavior(viewPrivate->style()->preferredLineCountBehavior());
        }
        QSizeF size = sizeForWidth(width, stringVariants.first(), preferredLineCount, preferredLineCountBehavior); //Get the size of the largest variant
        return size;
    }
    case Qt::MaximumSize: {
        return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    }
    default:
        qWarning("MLabel::sizeHint() don't know how to handle the value of 'which' ");
    }

    return QSizeF(0, 0);
}

void MLabelViewSimple::setupModel()
{
    viewPrivate->textOptions.setAlignment(viewPrivate->model()->alignment());
    updateStringVariants();
    if (viewPrivate->model()->wordWrap()) {
        viewPrivate->textOptions.setWrapMode(viewPrivate->model()->wrapMode());
    } else {
        viewPrivate->textOptions.setWrapMode(QTextOption::NoWrap);
    }
}

bool MLabelViewSimple::updateData(const QList<const char *>& modifications)
{
    const char *member = NULL;
    bool needUpdate = false;
    bool needStringVariantsUpdate = false;

    const MLabelModel *model = viewPrivate->model();

    foreach(member, modifications) {
        if (member == MLabelModel::Text) {
            if (!viewPrivate->controller->sizePolicy().hasHeightForWidth()) {
                preferredSize = QSizeF(-1, -1);
            }
            needStringVariantsUpdate = true;
            needUpdate = true;
        } else if (member == MLabelModel::Color) {
            needUpdate = true;
        } else if(member == MLabelModel::WrapMode) {
            if (model->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(model->wrapMode());
            }
            needStringVariantsUpdate = true;
            needUpdate = true;
        } else if (member == MLabelModel::TextElide) {
            needUpdate = true;
        } else if (member == MLabelModel::WordWrap) {
            if (model->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(model->wrapMode());
            } else {
                viewPrivate->textOptions.setWrapMode(QTextOption::NoWrap);
            }
            needStringVariantsUpdate = true;
            needUpdate = true;
        } else if (member == MLabelModel::TextDirection) {
            needUpdate = true;
        } else if (member == MLabelModel::Alignment) {
            viewPrivate->textOptions.setAlignment(viewPrivate->model()->alignment());
        } else if (member == MLabelModel::UseModelFont || member == MLabelModel::Font) {
            needUpdate = true;
        } else if (member == MLabelModel::PreferredLineCount) {
            needUpdate = true;
        } else if (member == MLabelModel::PreferredLineCountBehavior) {
            needUpdate = true;
        }
    }

    if (needStringVariantsUpdate)
        updateStringVariants();

    return needUpdate;
}

bool MLabelViewSimple::isRich()
{
    return false;
}

void MLabelViewSimple::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore(); //propagate link up to owner of label
}

void MLabelViewSimple::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void MLabelViewSimple::cancelEvent(MCancelEvent *event)
{
    Q_UNUSED(event);
}

void MLabelViewSimple::longPressEvent(QGestureEvent *event, QTapAndHoldGesture* gesture)
{
    event->ignore(gesture);
}

void MLabelViewSimple::orientationChangeEvent(MOrientationChangeEvent *event)
{
    Q_UNUSED(event);
}

void MLabelViewSimple::applyStyle()
{
    if (viewPrivate->model()->alignmentFromStyle())
        const_cast<MLabelModel*>(viewPrivate->model())->setAlignment(viewPrivate->style()->horizontalAlignment() | viewPrivate->style()->verticalAlignment());
    //preferredLineCount might have changed, but MLabelView::applyStyle will call updateGeometry() for us
}

QString MLabelViewSimple::renderedText() const
{
    const_cast<MLabelViewSimple*>(this)->initializeTextProperties();
    return staticText.text().replace(QChar::LineSeparator, QLatin1Char('\n')).replace(EllipsisChar, QLatin1String("..."));
}

bool MLabelViewSimple::tileInformation(int index, QPixmap &pixmap, QPoint &pos) const
{
    Q_UNUSED(index);
    Q_UNUSED(pixmap);
    Q_UNUSED(pos);
    return false;
}

void MLabelViewSimple::initializeTextProperties()
{
    if (!dirty) {
        return;
    }

    dirty = false;

    const MLabelStyle *style = viewPrivate->style();

    // First, assume the text direction is left to right.  The size and vertical position will be
    // correct, but possibly not the correct horizontal position.
    paintingRect = viewPrivate->boundingRect().adjusted(style->paddingLeft(), style->paddingTop(), -style->paddingRight(), -style->paddingBottom());

    // We now have the size that we want the text to fit into, so get the text to render
    bool isMultipleLines;
    const QString text = textToRender(paintingRect.size(), &isMultipleLines);

    // Now we can use the text and whether we have multiple lines, to determine how we want to align
    // the text.  This sets viewPrivate->textOptions.textDirection()
    viewPrivate->autoSetTextDirection(text, isMultipleLines);

    // Now that we have the correct textDirection we can fix the horizontal position of the
    // paintingRect
    if (viewPrivate->textOptions.textDirection() == Qt::RightToLeft) {
        qreal adjust = style->paddingRight() - style->paddingLeft();
        paintingRect.adjust(adjust, 0, -adjust, 0);
    }

    paintingRectWithMargins = paintingRect.adjusted(0, -style->paddingTop() - style->marginTop() - style->reactiveMarginTop(), 0, style->paddingBottom() + style->marginBottom() + style->reactiveMarginBottom());
    textOffset = paintingRect.topLeft().toPoint();

    staticText.setTextWidth(paintingRect.width());
    staticText.setTextOption(viewPrivate->textOptions);
    staticText.setText(text);
    staticText.prepare(QTransform(), viewPrivate->controller->font());

    adjustTextOffset();

    clip =  textOffset.x() < paintingRectWithMargins.x()
           || textOffset.y() < paintingRectWithMargins.y()
           || textOffset.x() + staticText.size().width() > paintingRectWithMargins.right()
           || textOffset.y() + staticText.size().height() > paintingRectWithMargins.bottom();

    pen = QPen(viewPrivate->textColor());
    font = QFont(viewPrivate->controller->font());
}

void MLabelViewSimple::updateStringVariants()
{
    QString text = viewPrivate->model()->text();

    // First convert the \r\n windows line breaks (if any) into \n ones
    text.replace(QLatin1String("\r\n"), QLatin1String("\n"));

    // QStaticText uses QTextLayout internally to render text. Contrary to
    // QPainter::drawText(const QRect &rectangle, ...) no pre-preparation of the
    // text is done (e. g. replace \n by QChar::LineSeparator or spaces dependent
    // on the wrapping). This is done manually here:
    if (wrap()) {
        text.replace(QLatin1Char('\n'), QChar::LineSeparator);
    } else {
        // The text should be shown in a single line. Replace all line breaks by a space.
        for (int i = 0; i < text.length(); ++i) {
            const QChar chr = text.at(i);
            if (chr == QLatin1Char('\n') || chr == QChar::LineSeparator) {
                text[i] = QLatin1Char(' ');
            }
        }
    }
    stringVariants = text.split(TextVariantSeparator);
    if (stringVariants.isEmpty()) //Shouldn't ever happen, but just incase
        stringVariants << "";
}

QString MLabelViewSimple::textToRender(const QSizeF &renderSize, bool *isMultipleLines) const
{

    return MElidedText::doElideText(renderSize,
                                    viewPrivate->controller->font(),
                                    stringVariants,
                                    viewPrivate->textOptions,
                                    isMultipleLines,
                                    viewPrivate->model()->textElide());
}

qreal MLabelViewSimple::restrictedTextWidth(const QString &text, qreal width) const
{
    qreal textWidth = -1.0;
    if (wrap()) {
        const QFontMetricsF metrics(viewPrivate->controller->font());
        textWidth = metrics.boundingRect(text).width();
        if (textWidth > width) {
            textWidth = width;
        }
    }
    return textWidth;
}

bool MLabelViewSimple::wrap() const
{
    const QTextOption::WrapMode wrapMode = viewPrivate->model()->wrapMode();
    return viewPrivate->controller->wordWrap()
           && (wrapMode != QTextOption::NoWrap)
           && (wrapMode != QTextOption::ManualWrap);
}

Qt::TextFlag MLabelViewSimple::textFlagForWrapMode() const
{
    Qt::TextFlag textFlag = Qt::TextSingleLine;
    if (viewPrivate->controller->wordWrap()) {
        const QTextOption::WrapMode wrapMode = viewPrivate->model()->wrapMode();
        if (wrapMode == QTextOption::WordWrap) {
            textFlag = Qt::TextWordWrap;
        } else if (wrapMode == QTextOption::WrapAnywhere || wrapMode == QTextOption::WrapAtWordBoundaryOrAnywhere) {
            textFlag = Qt::TextWrapAnywhere;
        }
    }
    return textFlag;
}

void MLabelViewSimple::adjustTextOffset()
{
    // textOptions.alignment() was mirrored by autoSetTextDirection() if RtoL (Arabic, Hebrew).
    // model->alignment() contains the raw (unmirrored) alignment.
    const Qt::Alignment alignment = viewPrivate->textOptions.alignment();

    if (paintingRect.width() < staticText.size().width()) {
        // There is not enough width available to show the text without clipping.
        // Assure that in this case the text will always be clipped on the right
        // in the L2R case. In the R2l case the text will always be clipped on the left.
        const qreal diff = staticText.size().width() - paintingRect.width();
        if (viewPrivate->controller->layoutDirection() == Qt::RightToLeft) {
            if (alignment & Qt::AlignLeft) {
                textOffset.rx() -= diff;
            } else if (alignment & Qt::AlignHCenter) {
                textOffset.rx() -= diff / 2.0;
            }
        } else if (alignment & Qt::AlignHCenter) {
            textOffset.rx() += diff / 2.0;
        } else if (alignment & Qt::AlignRight) {
            textOffset.rx() += diff;
        }
    }

    // Adjust y-position dependent on the vertical alignment
    if (alignment & Qt::AlignVCenter) {
        textOffset.ry() += (paintingRect.height() - staticText.size().height()) / 2.f;
    } else if (alignment & Qt::AlignBottom) {
        textOffset.setY(paintingRect.bottom() - staticText.size().height());
    }
}

QSizeF MLabelViewSimple::sizeForWidth(qreal width, const QString &text, int maximumLineCount, int preferredLineCountBehavior) const
{
    const_cast<MLabelViewSimple*>(this)->initializeTextProperties();

    if (maximumLineCount < 0) {
        const bool equalWidth = (width < 0.0 && staticText.textWidth() < 0.0) || width == staticText.textWidth();
        if (equalWidth && staticText.text() == text)
            return staticText.size();
    } else if (maximumLineCount == 0) {
        return QSizeF(0,0);
    }

    QTextLayout textLayout(text, font);
    textLayout.setTextOption(viewPrivate->textOptions);

    textLayout.beginLayout();
    int lineCount = 0;
    QFontMetricsF fm(font);
    qreal leading = fm.leading();
    qreal height = -leading;
    forever {
        QTextLine line = textLayout.createLine();
        if (!line.isValid())
            break;

        if (width >= 0)
            line.setLineWidth(width);
        else
            line.setLineWidth(INT_MAX);
        height += leading;
        line.setPosition(QPointF(0.0, height));
        height += line.height();
        lineCount++;

        if (maximumLineCount >= 0 && lineCount >= maximumLineCount) {
            if (width < 0) {
                //We need to work out the correct width, because we might want to an eliding
                //character on the end of the text
                QTextLine nextLine = textLayout.createLine();
                if (nextLine.isValid()) {
                    //We need to elide
                    QRectF lastLineRect = fm.boundingRect(text.mid(line.textStart(), line.textLength()) + EllipsisChar);
                    textLayout.endLayout();
                    return textLayout.boundingRect().united(lastLineRect).size();
                }
            }
            break;
        }
    }
    textLayout.endLayout();

    QSizeF size = textLayout.boundingRect().size();
    if (preferredLineCountBehavior == MLabel::LineCountSetsPreferredHeight && lineCount < maximumLineCount) {
        size.rheight() += fm.lineSpacing() * (maximumLineCount - lineCount); //See mlabelview_rich.cpp sizeHint() comments
    }
    return size;
}

void MLabelViewSimple::markDirty()
{
    dirty = true;
}
