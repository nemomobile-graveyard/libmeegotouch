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
            return QSizeF(fm.width(QLatin1Char('x')), fm.height());
        }
        //If word wrap and eliding are both disabled (the default) then use the smallest string as
        //the minimum size
        return sizeForWidth(-1, stringVariants.last()); //Get the size of the smallest variant
    }
    case Qt::PreferredSize: {
        qreal width = constraint.width();
        if (!viewPrivate->controller->sizePolicy().hasHeightForWidth()) {
            if (preferredSize.width() >= 0 && (preferredSize.width() < width || width < 0))
                width = preferredSize.width();
        }
        return sizeForWidth(width, stringVariants.first()); //Get the size of the largest variant
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
    viewPrivate->autoSetTextDirection();
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

    const MLabelModel *model = viewPrivate->model();

    foreach(member, modifications) {
        if (member == MLabelModel::Text) {
            if (!viewPrivate->controller->sizePolicy().hasHeightForWidth()) {
                preferredSize = QSizeF(-1, -1);
            }
            viewPrivate->autoSetTextDirection();
            updateStringVariants();
            needUpdate = true;
        } else if (member == MLabelModel::Color) {
            needUpdate = true;
        } else if(member == MLabelModel::WrapMode) {
            if (model->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(model->wrapMode());
            }
            needUpdate = true;
        } else if (member == MLabelModel::TextElide) {
            needUpdate = true;
        } else if (member == MLabelModel::WordWrap) {
            if (model->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(model->wrapMode());
            } else {
                viewPrivate->textOptions.setWrapMode(QTextOption::NoWrap);
            }
            needUpdate = true;
        } else if (member == MLabelModel::TextDirection) {
            viewPrivate->autoSetTextDirection();
            needUpdate = true;
        } else if (member == MLabelModel::Alignment) {
            viewPrivate->textOptions.setAlignment(viewPrivate->model()->alignment());
        } else if (member == MLabelModel::UseModelFont || member == MLabelModel::Font) {
            needUpdate = true;
        }
    }
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
}

QString MLabelViewSimple::renderedText() const
{
    return staticText.text();
}

bool MLabelViewSimple::tileInformation(int index, QPixmap &pixmap, int &y) const
{
    Q_UNUSED(index);
    Q_UNUSED(pixmap);
    Q_UNUSED(y);
    return false;
}

void MLabelViewSimple::initializeTextProperties()
{
    if (!dirty) {
        return;
    }

    dirty = false;

    const MLabelStyle *style = viewPrivate->style();

    if (viewPrivate->textOptions.textDirection() == Qt::LayoutDirectionAuto)
        viewPrivate->autoSetTextDirection();

    paintingRect = (viewPrivate->textOptions.textDirection() == Qt::LeftToRight)
        ? viewPrivate->boundingRect().adjusted(style->paddingLeft(), style->paddingTop(), -style->paddingRight(), -style->paddingBottom())
        : viewPrivate->boundingRect().adjusted(style->paddingRight(), style->paddingTop(), -style->paddingLeft(), -style->paddingBottom());
    paintingRectWithMargins = paintingRect.adjusted(0, -style->paddingTop() - style->marginTop() - style->reactiveMarginTop(), 0, style->paddingBottom() + style->marginBottom() + style->reactiveMarginBottom());
    textOffset = paintingRect.topLeft().toPoint();

    const QString text = textToRender(paintingRect.size());

    staticText.setTextWidth(paintingRect.width());
    staticText.setTextOption(viewPrivate->textOptions);
    staticText.setText(text);
    staticText.prepare(QTransform(), viewPrivate->controller->font());

    adjustTextOffset();

    clip =  textOffset.x() < paintingRectWithMargins.x()
           || textOffset.y() < paintingRectWithMargins.y()
           || textOffset.x() + staticText.size().width() > paintingRectWithMargins.right()
           || textOffset.y() + staticText.size().height() > paintingRectWithMargins.bottom();

    const QColor& color = viewPrivate->model()->color();
    pen = QPen(color.isValid() ? color : style->color());
    font = QFont(viewPrivate->controller->font());
}

void MLabelViewSimple::updateStringVariants()
{
    QString text = viewPrivate->model()->text();
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
    if (stringVariants.isEmpty()) //Shouldn't ever happen, but just incase:w
        stringVariants << "";
}

QString MLabelViewSimple::textToRender(const QSizeF &renderSize) const
{
    QString text;
    if (stringVariants.size() > 1) {
        // The text consists of several strings. Find the first string that fits into the
        // available width. If no string has been found, the last string will be used.
        const QFontMetricsF metrics(viewPrivate->controller->font());
        foreach (const QString &string, stringVariants) {
            text = string;
            if (wrap()) {
                QSizeF stringSize = metrics.boundingRect(QRectF(QPointF(0,0), renderSize),
                                                         Qt::TextWordWrap, string).size();
                if (stringSize.width() <= renderSize.width()
                        && stringSize.height() <= renderSize.height())
                    break;
            } else if (metrics.width(text) <= renderSize.width()) {
                break;
            }
        }
    } else {
        text = stringVariants.first();
    }

    if (viewPrivate->model()->textElide() && text.size() > 4) {
        if (!wrap()) {
            QFontMetrics metrics(viewPrivate->controller->font());
            //No word wrap, so simple to elide
            text = metrics.elidedText(text, Qt::ElideRight, renderSize.width());
        } else {
            //For word wrapping and eliding, bisect to find where to elide
            QStaticText staticText2;
            staticText2.setTextOption(staticText.textOption());
            staticText2.setTextWidth(renderSize.width());
            staticText2.setText(text);
            staticText2.prepare(QTransform(), viewPrivate->controller->font());

            if (staticText2.size().height() > renderSize.height() || staticText2.size().width() > renderSize.width()) {
                int bad = text.length();
                int good = 0;
                int offset = bad + (good - bad)/2;
                while (offset != good && offset != bad) {
                    staticText2.setText(text.left(offset) + "...");
                    if (staticText2.size().height() > renderSize.height() || staticText2.size().width() > renderSize.width())
                        bad = offset;
                    else
                        good = offset;
                    offset = bad + (good - bad)/2;
                }
                text = text.left(good) + "...";
            }
        }
    }

    return text;
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

QSizeF MLabelViewSimple::sizeForWidth(qreal width, const QString &text) const
{
    const_cast<MLabelViewSimple*>(this)->initializeTextProperties();

    const bool equalWidth = (width < 0.0 && staticText.textWidth() < 0.0) || width == staticText.textWidth();

    if (equalWidth && staticText.text() == text)
        return staticText.size();

    QStaticText staticText2(staticText);
    staticText2.setTextOption(staticText.textOption());  // TODO: remove after Qt-bug #13368 has been fixed
    staticText2.setTextWidth(width);
    if (staticText2.text() != text)
        staticText2.setText(text);
    staticText2.prepare(QTransform(), font);

    return staticText2.size();
}

void MLabelViewSimple::markDirty()
{
    dirty = true;
}
