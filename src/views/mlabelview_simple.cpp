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


MLabelViewSimple::MLabelViewSimple(MLabelViewPrivate *viewPrivate) :
    viewPrivate(viewPrivate), preferredSize(-1, -1), dirty(true), staticText()
{
}

MLabelViewSimple::~MLabelViewSimple()
{
}

void MLabelViewSimple::drawContents(QPainter *painter, const QSizeF &size)
{
    Q_UNUSED(size);
   
    initializeStaticText();
    
    const MLabelModel *model = viewPrivate->model();
    const MLabelStyle *style = viewPrivate->style();

    painter->setPen(model->color().isValid() ? model->color() : style->color());
    painter->setFont(viewPrivate->controller->font());
    painter->drawStaticText(textOffset, staticText);    
}

bool MLabelViewSimple::resizeEvent(QGraphicsSceneResizeEvent *event)
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

    QRectF bR = fm.boundingRect(QRectF(QPoint(0, 0), event->newSize()), 
                                viewPrivate->textOptions.alignment() | wrap(), viewPrivate->model()->text());
    if (bR.height() > fm.height()) {
        preferredSize = QSizeF(bR.width(), bR.height());
        return true;
    } else
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

        QRectF bR(fm.boundingRect(r, viewPrivate->textOptions.alignment() | wrap(),
                                  viewPrivate->model()->text()));

        return QSizeF(fm.width("x"), bR.height());
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

        QRectF bR(fm.boundingRect(QRectF(0, 0, w, h), viewPrivate->textOptions.alignment() | wrap(),
                                  viewPrivate->model()->text()));
        return bR.size().boundedTo(QSizeF(w, h));
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
}

bool MLabelViewSimple::updateData(const QList<const char *>& modifications)
{
    const char *member = NULL;
    bool needUpdate = false;

    const MLabelModel *model = viewPrivate->model();

    foreach(member, modifications) {
        if (member == MLabelModel::Text) {
            preferredSize = QSizeF(-1, -1);
            needUpdate = true;
        } else if(member == MLabelModel::WrapMode) {
            if (model->wordWrap()) {
                if (model->wrapMode() == QTextOption::NoWrap) {
                    //Note that NoWrap works incorrectly in Qt 4.6
                    viewPrivate->textOptions.setWrapMode(QTextOption::ManualWrap);
                } else {
                    viewPrivate->textOptions.setWrapMode(model->wrapMode());
                }
            }
            needUpdate = true;
        } else if (member == MLabelModel::WordWrap) {
            if (model->wordWrap()) {
                viewPrivate->textOptions.setWrapMode(model->wrapMode());
            } else {
                //Note that NoWrap works incorrectly in Qt 4.6
                viewPrivate->textOptions.setWrapMode(QTextOption::ManualWrap);
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

void MLabelViewSimple::longPressEvent(QGraphicsSceneContextMenuEvent *event)
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
    
    const QRectF paintingRect(viewPrivate->boundingRect().adjusted(style->paddingLeft(), style->paddingTop(), -style->paddingRight(), -style->paddingBottom()));
    textOffset = paintingRect.topLeft().toPoint();
    
    QString textToRender = model->text();
    if (model->textElide() && textToRender.size() > 4) {
        QFontMetrics fm(viewPrivate->controller->font());
        textToRender = fm.elidedText(model->text(), Qt::ElideRight, paintingRect.width());
    }
    
    const qreal textWidth = viewPrivate->controller->wordWrap() ? paintingRect.width() : -1.0;
    staticText.setTextWidth(textWidth);
    staticText.setText(textToRender);
    staticText.prepare(QTransform(), viewPrivate->controller->font());
    
    const Qt::Alignment alignment = viewPrivate->textOptions.alignment();
    
    // Adjust x-position dependent on the horizontal alignment
    if (alignment & Qt::AlignHCenter) {
        const qreal inc = (paintingRect.width() - staticText.size().width()) / 2.0;
        if (inc > 0.0) {
            textOffset.rx() += inc;
        }
    } else if (alignment & Qt::AlignRight) {
        textOffset.setX(paintingRect.right() - staticText.size().width());    
    }
    
    // Adjust y-position dependent on the vertical alignment
    if (alignment & Qt::AlignVCenter) {
        const qreal inc = (paintingRect.height() - staticText.size().height()) / 2.0;
        if (inc > 0.0) {
            textOffset.ry() += inc;
        }
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
    
