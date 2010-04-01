/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#include "duilabelview.h"
#include "duilabelview_p.h"
#include "duilabelmodel.h"
#include "duilabel.h"
#include "duiviewcreator.h"

#include <QPainter>
#include <QTextDocument>
#include <QTextCursor>
#include <QFontMetrics>
#include <QPixmapCache>
#include <QAbstractTextDocumentLayout>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsSceneResizeEvent>

DuiLabelViewSimple::DuiLabelViewSimple(DuiLabelViewPrivate *viewPrivate) :
    viewPrivate(viewPrivate), preferredSize(-1, -1), dirty(true), cachedElidedText("")
{
}

DuiLabelViewSimple::~DuiLabelViewSimple()
{
}

#ifdef __arm__
void DuiLabelViewSimple::drawContents(QPainter *painter, const QSizeF &size)
{
    Q_UNUSED(size);

    const DuiLabelModel *model = viewPrivate->model();
    const DuiLabelStyle *style = viewPrivate->style();
    QRectF paintingRect(viewPrivate->boundingRect().adjusted(style->paddingLeft(), style->paddingTop(), -style->paddingRight(), -style->paddingBottom()));
    QString textToRender = model->text();
    if (model->textElide() && textToRender.size() > 4) {
        if(cachedElidedText.isEmpty()) {
            QFontMetrics fm(viewPrivate->controller->font());
            cachedElidedText = fm.elidedText(model->text(), Qt::ElideRight, paintingRect.width());
        }

        textToRender = cachedElidedText;
    }

    if (textToRender.isEmpty()) {
        return;
    }

    painter->setFont(viewPrivate->controller->font());
    painter->setPen(model->color().isValid() ? model->color() : style->color());
    painter->setLayoutDirection(model->textDirection());
    painter->drawText(paintingRect, viewPrivate->textOptions.alignment() | Qt::TextSingleLine, textToRender);
}

#else

void DuiLabelViewSimple::drawContents(QPainter *painter, const QSizeF &size)
{
    Q_UNUSED(size);

    painter->drawImage(textOffset, generateImage());
}

#endif

QPixmap DuiLabelViewSimple::generatePixmap()
{
    return QPixmap();
}

QImage DuiLabelViewSimple::generateImage()
{
    if(!dirty)
        return cachedImage;

    dirty = false;
    const DuiLabelModel *model = viewPrivate->model();
    const DuiLabelStyle *style = viewPrivate->style();
    QRectF paintingRect(viewPrivate->boundingRect().adjusted(style->paddingLeft(), style->paddingTop(), -style->paddingRight(), -style->paddingBottom()));
    QSizeF paintingRectSize = paintingRect.size();
    textOffset = paintingRect.topLeft().toPoint();
    QString textToRender = model->text();
    if (model->textElide() && textToRender.size() > 4) {
        QFontMetrics fm(viewPrivate->controller->font());
        textToRender = fm.elidedText(model->text(), Qt::ElideRight, paintingRect.width());
    }

    if (textToRender.isEmpty()) {
        cachedImage = QImage();
        return cachedImage;
    }

    if(cachedImage.size() != paintingRect.size().toSize())
    {
        cachedImage = QImage(paintingRect.size().toSize(), QImage::Format_ARGB32_Premultiplied);
    }

    QImage &image = cachedImage;

    if (!image.isNull()) {
        image.fill(0);
        QPainter painter(&image);
        painter.setRenderHint(QPainter::TextAntialiasing);
        painter.setFont(viewPrivate->controller->font());
        painter.setPen(model->color().isValid() ? model->color() : style->color());
        painter.setLayoutDirection(model->textDirection());

        painter.drawText(0, 0, paintingRectSize.width(), paintingRectSize.height(), viewPrivate->textOptions.alignment() | Qt::TextSingleLine, textToRender);
    }

    return image;
}

bool DuiLabelViewSimple::resizeEvent(QGraphicsSceneResizeEvent *event)
{
    // There is no way to specify sizeHint for a text without knowing possible width.
    // 1st phase, when Qt calls sizeHint, view will return approximate values for
    // minimum and preferred size. When resizeEvent comes, layout already knows
    // sizes of components, and here comes
    // 2nd phase, when we identify widget's height, based on width. Our height will
    // change and we don't want to occupy more space then need, so we have to call
    // updateGeometry, to tell layout to update sizeHint cache. This function
    // return true if such update is needed.

    QFontMetricsF fm(viewPrivate->controller->font());
    QRectF bR = fm.boundingRect(QRectF(QPoint(0, 0), event->newSize()), viewPrivate->textOptions.alignment(), viewPrivate->model()->text());
    if (bR.height() > fm.height()) {
        preferredSize = QSizeF(bR.width(), bR.height());
        return true;
    } else
        return false;
}

QSizeF DuiLabelViewSimple::sizeHint(Qt::SizeHint which, const QSizeF &constraint) const
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

        QRectF bR(fm.boundingRect(r, viewPrivate->textOptions.alignment() | Qt::TextSingleLine,
                                  viewPrivate->model()->text()));

        return QSizeF(fm.width(""), bR.height());
    }
    case Qt::PreferredSize: {
        qreal w = constraint.width();
        qreal h = constraint.height();
        if (w < 0) {
            w = QWIDGETSIZE_MAX;
        }
        if (h < 0) {
            h = QWIDGETSIZE_MAX;
        }
        if (preferredSize.width() >= 0 && preferredSize.width() < w)
            w = preferredSize.width();
        if (preferredSize.height() >= 0 && preferredSize.height() < h)
            h = preferredSize.height();

        QFontMetricsF fm(viewPrivate->controller->font());
        QRectF bR(fm.boundingRect(QRectF(0, 0, w, h), viewPrivate->textOptions.alignment() | Qt::TextSingleLine,
                                  viewPrivate->model()->text()));
        return bR.size().boundedTo(QSizeF(w, h));
    }
    case Qt::MaximumSize: {
        return QSizeF(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    }
    default:
        qWarning("DuiLabel::sizeHint() don't know how to handle the value of 'which' ");
    }

    return QSizeF(0, 0);
}

void DuiLabelViewSimple::setupModel()
{
    viewPrivate->textOptions.setTextDirection(viewPrivate->model()->textDirection());
    viewPrivate->textOptions.setAlignment(viewPrivate->model()->alignment());
}

bool DuiLabelViewSimple::updateData(const QList<const char *>& modifications)
{
    const char *member = NULL;
    bool needUpdate = false;

    foreach(member, modifications) {
        if (member == DuiLabelModel::Text) {
            preferredSize = QSizeF(-1, -1);
            needUpdate = true;
        } else if (member == DuiLabelModel::WordWrap) {
            if (viewPrivate->model()->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(QTextOption::WordWrap);
            } else {
                viewPrivate->textOptions.setWrapMode(QTextOption::ManualWrap);
            }
        } else if (member == DuiLabelModel::TextDirection) {
            viewPrivate->textOptions.setTextDirection(viewPrivate->model()->textDirection());
        } else if (member == DuiLabelModel::Alignment) {
            viewPrivate->textOptions.setAlignment(viewPrivate->model()->alignment());
        } else if (member == DuiLabelModel::UseModelFont || member == DuiLabelModel::Font) {
            needUpdate = true;
        }
    }
    return needUpdate;
}

bool DuiLabelViewSimple::isRich()
{
    return false;
}

void DuiLabelViewSimple::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    event->ignore(); //propagate link up to owner of label
}

void DuiLabelViewSimple::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
    Q_UNUSED(event);
}

void DuiLabelViewSimple::cancelEvent(DuiCancelEvent *event)
{
    Q_UNUSED(event);
}

void DuiLabelViewSimple::longPressEvent(QGraphicsSceneContextMenuEvent *event)
{
    Q_UNUSED(event);
}

void DuiLabelViewSimple::applyStyle()
{
}

void DuiLabelViewSimple::markDirty()
{
    dirty = true;
    cachedElidedText = "";
}
