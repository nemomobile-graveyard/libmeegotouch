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


MLabelViewSimple::MLabelViewSimple(MLabelViewPrivate *viewPrivate) :
    viewPrivate(viewPrivate), textOffset(), paintingRect(), dirty(true), staticText()
{
    staticText.setTextFormat(Qt::PlainText);
}

MLabelViewSimple::~MLabelViewSimple()
{
}

void MLabelViewSimple::drawContents(QPainter *painter, const QSizeF &size)
{
    Q_UNUSED(size);

    initializeStaticText();
    if (staticText.text().isEmpty() || paintingRect.isEmpty()) {
        return;
    }

    const MLabelModel *model = viewPrivate->model();
    const MLabelStyle *style = viewPrivate->style();

    painter->setPen(model->color().isValid() ? model->color() : style->color());
    painter->setFont(viewPrivate->controller->font());
    painter->setRenderHint(QPainter::TextAntialiasing);
    painter->setLayoutDirection(model->textDirection());

    const bool clip =    textOffset.x() < paintingRect.x()
                      || textOffset.y() < paintingRect.y()
                      || textOffset.x() + staticText.size().width()  > paintingRect.right()
                      || textOffset.y() + staticText.size().height() > paintingRect.bottom();

    if (clip) {
        painter->save();
        painter->setClipRect(paintingRect, Qt::IntersectClip);
        painter->drawStaticText(textOffset, staticText);
        painter->restore();
    } else {
        painter->drawStaticText(textOffset, staticText);
    }
}

bool MLabelViewSimple::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    Q_UNUSED(event);
    return false;
}

QSizeF MLabelViewSimple::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
{
    switch (which) {
    case Qt::MinimumSize: {
        QFontMetricsF fm(viewPrivate->controller->font());
        QRectF r(0, 0, constraint.width(), constraint.height());

        if (r.width() < 0) {
            r.setWidth(QWIDGETSIZE_MAX);
        }
        if (r.height() < 0) {
            r.setHeight(QWIDGETSIZE_MAX);
        }

        QSizeF s = fm.boundingRect(r, viewPrivate->textOptions.alignment() | wrap(),
                                  viewPrivate->model()->text()).size();
        s.setWidth(fm.width("x"));
        return s;
    }
    case Qt::PreferredSize: {
        const_cast<MLabelViewSimple*>(this)->initializeStaticText();
        qreal w = constraint.width();

        //A negative constraint of means unbounded for both constraint and staticText
        if ((w < 0 && staticText.textWidth() < 0) && w == staticText.textWidth()) {
            return staticText.size();
        }

        QStaticText staticText2(staticText);
        staticText2.setTextOption(staticText.textOption());  // TODO: remove after Qt-bug #13368 has been fixed
        staticText2.setTextWidth(w);
        return staticText2.size();
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
    viewPrivate->textOptions.setTextDirection(viewPrivate->model()->textDirection());
    viewPrivate->textOptions.setAlignment(viewPrivate->model()->alignment());
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
            needUpdate = true;
        } else if (member == MLabelModel::Color) {
            needUpdate = true;
        } else if(member == MLabelModel::WrapMode) {
            if (model->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(model->wrapMode());
            }
            needUpdate = true;
        } else if (member == MLabelModel::WordWrap) {
            if (model->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(model->wrapMode());
            } else {
                viewPrivate->textOptions.setWrapMode(QTextOption::NoWrap);
            }
            needUpdate = true;
        } else if (member == MLabelModel::TextDirection) {
            needUpdate = true;
            viewPrivate->textOptions.setTextDirection(viewPrivate->model()->textDirection());
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
}

void MLabelViewSimple::initializeStaticText()
{
    if (!dirty) {
        return;
    }

    dirty = false;

    const MLabelModel *model = viewPrivate->model();
    const MLabelStyle *style = viewPrivate->style();

    paintingRect = (viewPrivate->textOptions.alignment() == Qt::LeftToRight)
        ? viewPrivate->boundingRect().adjusted(style->paddingLeft(), style->paddingTop(), -style->paddingRight(), -style->paddingBottom())
        : viewPrivate->boundingRect().adjusted(style->paddingRight(), style->paddingTop(), -style->paddingLeft(), -style->paddingBottom());
    textOffset = paintingRect.topLeft().toPoint();

    QString textToRender = model->text();

    const QChar multiLengthSeparator(0x9c, 0);
    if (textToRender.contains(multiLengthSeparator)) {
        // The text consists of several strings. Find the first string that fits into the
        // available width. If no string has been found, the last string will be used.
        const QStringList strings = textToRender.split(multiLengthSeparator);
        const QFontMetricsF metrics(viewPrivate->controller->font());
        foreach (const QString &string, strings) {
            textToRender = string;
            if (metrics.width(textToRender) <= paintingRect.width()) {
                break;
            }
        }
    }

    if (model->textElide() && textToRender.size() > 4) {
        QFontMetrics metrics(viewPrivate->controller->font());
        textToRender = metrics.elidedText(textToRender, Qt::ElideRight, paintingRect.width());
    }

    // QStaticText uses QTextLayout internally to render text. In opposite to
    // QPainter::drawText(const QRect &rectangle, ...) no pre-preparation of the
    // text is done (e. g. replace \n by QChar::LineSeparator or spaces dependent
    // on the wrapping). This is done manually here:
    qreal textWidth = -1.0;
    const QTextOption::WrapMode wrapMode = model->wrapMode();
    const bool singleLine = !viewPrivate->controller->wordWrap()
                            || (wrapMode == QTextOption::NoWrap)
                            || (wrapMode == QTextOption::ManualWrap);
    if (singleLine) {
        // Replace all line breaks by a space
        for (int i = 0; i < textToRender.length(); ++i) {
            const QChar chr = textToRender.at(i);
            if (chr == QLatin1Char('\n') || chr == QChar::LineSeparator) {
                textToRender[i] = QLatin1Char(' ');
            }
        }
    } else {
        textToRender.replace(QLatin1Char('\n'), QChar::LineSeparator);

        const QFontMetricsF metrics(viewPrivate->controller->font());
        textWidth = metrics.width(textToRender);
        if (textWidth > paintingRect.width()) {
            textWidth = paintingRect.width();
        }
    }

    staticText.setTextWidth(textWidth);

    staticText.setTextOption(viewPrivate->textOptions);
    staticText.setText(textToRender);
    staticText.prepare(QTransform(), viewPrivate->controller->font());

    adjustTextOffset();
}

void MLabelViewSimple::adjustTextOffset()
{
    const MLabelModel *model = viewPrivate->model();
    Qt::Alignment alignment = viewPrivate->textOptions.alignment();
    if (model->textDirection() == Qt::RightToLeft && !(alignment & Qt::AlignHCenter)) {
        // Mirror the horizontal alignment
        if (alignment & Qt::AlignRight) {
            alignment = (alignment | Qt::AlignLeft) & ~Qt::AlignRight;
        } else {
            alignment = (alignment | Qt::AlignRight) & ~Qt::AlignLeft;
        }
    }

    // Adjust x-position dependent on the horizontal alignment
    if (alignment & Qt::AlignHCenter) {
        textOffset.rx() += (paintingRect.width() - staticText.size().width()) / 2.0;
    } else if (alignment & Qt::AlignRight) {
        textOffset.setX(paintingRect.right() - staticText.size().width());
    }

    // Adjust y-position dependent on the vertical alignment
    if (alignment & Qt::AlignVCenter) {
        textOffset.ry() += (paintingRect.height() - staticText.size().height()) / 2.0;
    } else if (alignment & Qt::AlignBottom) {
        textOffset.setY(paintingRect.bottom() - staticText.size().height());
    }
}

void MLabelViewSimple::markDirty()
{
    dirty = true;
}

Qt::TextFlag MLabelViewSimple::wrap() const
{
    if(!viewPrivate->controller->wordWrap()) {
        return Qt::TextSingleLine;
    }

    QTextOption::WrapMode wrapMode = viewPrivate->model()->wrapMode();
    if (wrapMode == QTextOption::WordWrap) {
        return Qt::TextWordWrap;
    } else if (wrapMode == QTextOption::WrapAnywhere || wrapMode == QTextOption::WrapAtWordBoundaryOrAnywhere) {
        return Qt::TextWrapAnywhere;
    }
    return Qt::TextSingleLine;
}

