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
 

#include "mlistindextooltip.h"
#include "mlistindextooltip_p.h"

#include <MLabel>
#include <MSeparator>

#include <QGraphicsLinearLayout>
#include <QPropertyAnimation>

MListIndexTooltipPrivate::MListIndexTooltipPrivate()
    : q_ptr(0),
    layout(NULL),
    arrowOffset(0.0),
    arrowOffsetAnimation(NULL),
    snapAnimation(NULL),
    panel(NULL),
    panelLayout(NULL)
{
    arrowOffsetAnimation = new QPropertyAnimation;
    snapAnimation = new QPropertyAnimation;
}

MListIndexTooltipPrivate::~MListIndexTooltipPrivate()
{
    delete snapAnimation;
    delete arrowOffsetAnimation;
}

void MListIndexTooltipPrivate::initLayout()
{
    Q_Q(MListIndexTooltip);

    if (!layout) {
        layout = new QGraphicsLinearLayout(Qt::Vertical, q);
        layout->setContentsMargins(0, 0, 0, 0);
        layout->setSpacing(0);
    }

    panel = new MWidget(q);
    panelLayout = new QGraphicsLinearLayout(Qt::Vertical, panel);
    panelLayout->setContentsMargins(0, 0, 0, 0);
    panelLayout->setSpacing(0);

    layout->addItem(panel);
    q->disconnect(q, SLOT(_q_updateSizeToCentralWidget()));
    q->connect(panel, SIGNAL(geometryChanged()), q, SLOT(_q_updateSizeToCentralWidget()));
    _q_updateSizeToCentralWidget();
}

void MListIndexTooltipPrivate::initAnimations()
{
    Q_Q(MListIndexTooltip);

    snapAnimation->setTargetObject(panel);
    snapAnimation->setPropertyName("paintOffset");

    arrowOffsetAnimation->setTargetObject(q);
    arrowOffsetAnimation->setPropertyName("arrowOffset");
    arrowOffsetAnimation->setEndValue(0);
}

void MListIndexTooltipPrivate::_q_updateSizeToCentralWidget()
{
    Q_Q(MListIndexTooltip);

    const MListIndexTooltipStyle *s = static_cast<const MListIndexTooltipStyle *>(q->style().operator ->());
    QSizeF parentSize = panel->size();
    parentSize += QSizeF(s->paddingLeft()+s->paddingRight(), s->paddingTop()+s->paddingBottom());

    q->resize(parentSize);
    if (snapAnimation->state() == QPropertyAnimation::Running)
        snapAnimation->setEndValue(QPointF(s->paddingLeft(), s->paddingTop()));
    else
        panel->setPos(s->paddingLeft(), s->paddingTop());
}

void MListIndexTooltipPrivate::clearIndexes()
{
    qDeleteAll(indexLabels);
    indexLabels.clear();

    for (int i = panelLayout->count(); i > 0; i--) {
        QGraphicsLayoutItem *item = panelLayout->itemAt(i - 1);
        panelLayout->removeItem(item);
        delete item;
    }
}

void MListIndexTooltipPrivate::createIndexes(int count)
{
    if (count == indexLabels.count())
        return;

    clearIndexes();
    for (int i = 0; i < count; i++) {
        MLabel *label = new MLabel(panel);
        label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        label->setStyleName("IndexLabel");
        label->setTextElide(true);

        panelLayout->addItem(label);

        if (i != count - 1) {
            MSeparator *separator = new MSeparator(panel, Qt::Horizontal);
            panelLayout->addItem(separator);
            separator->setStyleName("IndexSeparator");
        }
        indexLabels.append(label);
    }
}

void MListIndexTooltipPrivate::setIndexText(int index, const QString &text)
{
    indexLabels[index]->setText(text);
}

void MListIndexTooltipPrivate::setIndexSelected(int index)
{
    for (int i = 0; i < indexLabels.count(); i++) {
        if (i == index) {
            indexLabels[i]->setSelected(true);
            setArrowOffsetAnimated(calculateArrowOffset(i));
        }
        else
            indexLabels[i]->setSelected(false);
    }
}

void MListIndexTooltipPrivate::setArrowOffsetAnimated(qreal offset)
{
    Q_Q(MListIndexTooltip);

    if (arrowOffsetAnimation->endValue().toReal() != offset) {
        arrowOffsetAnimation->setEndValue(q->arrowOffset());
        arrowOffsetAnimation->stop();

        arrowOffsetAnimation->setStartValue(q->arrowOffset());
        arrowOffsetAnimation->setEndValue(offset);
        arrowOffsetAnimation->setDuration(100);
        arrowOffsetAnimation->start();
    }
}

qreal MListIndexTooltipPrivate::calculateArrowOffset(int index)
{
    MLabel *indexLabel = indexLabels[index];
    return indexLabel->pos().y() + indexLabel->size().height() / 2;
}

MListIndexTooltip::MListIndexTooltip(QGraphicsItem *parent)
    : MStylableWidget(parent),
    d_ptr(new MListIndexTooltipPrivate)
{
    Q_D(MListIndexTooltip);
    d->q_ptr = this;

    d->initLayout();
    d->initAnimations();
}

MListIndexTooltip::~MListIndexTooltip()
{
    delete d_ptr;
}

void MListIndexTooltip::setIndexCount(int count)
{
    Q_D(MListIndexTooltip);
    d->createIndexes(count);
}

void MListIndexTooltip::setIndexText(int index, const QString &text)
{
    Q_D(MListIndexTooltip);
    d->setIndexText(index, text);
}

void MListIndexTooltip::setIndexSelected(int index)
{
    Q_D(MListIndexTooltip);
    d->setIndexSelected(index);
}

void MListIndexTooltip::setArrowOffset(qreal offset)
{
    Q_D(MListIndexTooltip);
    d->arrowOffset = offset;
}

qreal MListIndexTooltip::arrowOffset() const
{
    Q_D(const MListIndexTooltip);
    return d->arrowOffset;
}

void MListIndexTooltip::drawForeground(QPainter *painter, const QStyleOptionGraphicsItem *option) const
{
    Q_D(const MListIndexTooltip);

    MStylableWidget::drawForeground(painter, option);

    const MListIndexTooltipStyle *s = static_cast<const MListIndexTooltipStyle *>(style().operator ->());
    if (s->arrowPixmap() != NULL && !s->arrowPixmap()->isNull())
        painter->drawPixmap(size().width(), d->arrowOffset + s->paddingTop() - s->arrowPixmap()->size().height() / 2, *s->arrowPixmap());
}

void MListIndexTooltip::snap(qreal snapDistance)
{
    Q_D(MListIndexTooltip);

    const MListIndexTooltipStyle *s = static_cast<const MListIndexTooltipStyle *>(style().operator ->());

    d->snapAnimation->stop();
    d->snapAnimation->setTargetObject(d->panel);
    d->snapAnimation->setPropertyName("paintOffset");

    d->snapAnimation->setStartValue(QPointF(s->paddingLeft(), s->paddingTop() + snapDistance));
    d->snapAnimation->setEndValue(QPointF(s->paddingLeft(), s->paddingTop()));
    d->snapAnimation->setDuration(100);
    d->snapAnimation->start();
}

void MListIndexTooltip::applyStyle()
{
    Q_D(MListIndexTooltip);
    MStylableWidget::applyStyle();

    const MListIndexTooltipStyle *s = static_cast<const MListIndexTooltipStyle *>(style().operator ->());

    if (d->panel) {
        setMinimumSize(s->minimumSize());
        setPreferredSize(s->preferredSize());
        setMaximumSize(s->maximumSize());
        updateGeometry();
    }

    d->_q_updateSizeToCentralWidget();
}

#include "moc_mlistindextooltip.cpp"
