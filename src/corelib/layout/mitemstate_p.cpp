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

#include "mitemstate_p.h"
#include <QGraphicsWidget>

MItemState::MItemState(QGraphicsWidget *i) :
    m_item(i),
    m_sourceOpacity(-1),
    m_targetOpacity(-1),
    m_flags(MItemState::STATE_FLAG_NONE),
    m_geometryProgress(1),
    m_opacityProgress(1),
    m_isAnimationDone(true)
{
}

MItemState::~MItemState()
{
}

QGraphicsWidget *MItemState::item() const
{
    return m_item;
}

QRectF MItemState::sourceGeometry() const
{
    return m_sourceGeometry;
}

QRectF MItemState::targetGeometry() const
{
    return m_targetGeometry;
}

MItemState::Flags MItemState::flags() const
{
    return m_flags;
}

void MItemState::setGeometry(const QRectF &p)
{
    m_item->setGeometry(p);
    m_sourceGeometry = p;
    m_isAnimationDone = false;
    if (m_sourceGeometry != m_targetGeometry)
        m_geometryProgress = 0;
    else
        m_geometryProgress = 1;
}
void MItemState::setTargetGeometry(const QRectF &p)
{
    Q_ASSERT(m_item);
    QSizeF maximum  = m_item->effectiveSizeHint(Qt::MaximumSize, p.size());
    QSizeF minimum  = m_item->effectiveSizeHint(Qt::MinimumSize, p.size());
    QRectF target = p;
    target.setSize(target.size().boundedTo(maximum).expandedTo(minimum));
    if (target == m_targetGeometry && (isSet(STATE_FLAG_SHOWING) || isSet(STATE_FLAG_TO_BE_SHOWN)) && !isSet(STATE_FLAG_TO_BE_HIDDEN))
        return; //Nothing has changed and item is already being shown or is already showing.  If the item is hiding, then we need to reset the source and show the item anyway
    m_targetGeometry = target;
    m_sourceGeometry = m_item->geometry();

    if (!isSet(STATE_FLAG_SHOWING) || isSet(STATE_FLAG_TO_BE_HIDDEN)) //If it's not being shown or we are in the middle of a showing animation
        addFlags(STATE_FLAG_TO_BE_SHOWN);
    removeFlags(STATE_FLAG_TO_BE_HIDDEN);

    m_isAnimationDone = false;
    if (m_sourceGeometry != m_targetGeometry)
        m_geometryProgress = 0;
}

void MItemState::setFlags(Flags new_flags)
{
    m_flags = new_flags;
}

bool MItemState::isSet(Flag flag) const
{
    return (m_flags & flag) == flag;
}

void MItemState::addFlags(Flags additional_flags)
{
    m_flags |= additional_flags;
}

void MItemState::removeFlags(Flags flags_to_remove)
{
    m_flags &= ~flags_to_remove;
}

bool MItemState::isAnimationDone() const
{
    return m_isAnimationDone;
}

void MItemState::animationDone()
{
    m_isAnimationDone = true;
    m_opacityProgress = 1;
    m_geometryProgress = 1;
    m_sourceGeometry = m_targetGeometry;
    m_sourceOpacity = m_targetOpacity = -1;
    //Keep the deleted flag so that it can be deleted
    if (m_flags & STATE_FLAG_TO_BE_SHOWN) {
        m_flags &= ~STATE_FLAG_TO_BE_SHOWN;
        m_flags |= STATE_FLAG_SHOWING;
    }
    if (m_flags & STATE_FLAG_TO_BE_HIDDEN) {
        m_flags &= ~STATE_FLAG_TO_BE_HIDDEN;
        m_flags &= ~STATE_FLAG_SHOWING;
    }
}

void MItemState::setGeometryProgress(qreal progress)
{
    m_geometryProgress = progress;
    if (progress != 1)
        m_isAnimationDone = false;
}
void MItemState::setOpacityProgress(qreal progress)
{
    m_opacityProgress = progress;
    if (progress != 1)
        m_isAnimationDone = false;
}
qreal MItemState::geometryProgress() const
{
    return m_geometryProgress;
}
qreal MItemState::opacityProgress() const
{
    return m_opacityProgress;
}

void MItemState::setTargetOpacity(qreal opacity)
{
    Q_ASSERT(m_item);
    m_targetOpacity = opacity;
    if (m_targetOpacity == -1)
        return;

    m_sourceOpacity = m_item->opacity();
    if (m_sourceOpacity != -1 && m_targetOpacity != m_sourceOpacity) {
        m_opacityProgress = 0;
        m_isAnimationDone = false;
    }
}
qreal MItemState::targetOpacity() const
{
    return m_targetOpacity;
}
qreal MItemState::sourceOpacity() const
{
    return m_sourceOpacity;
}
void MItemState::hide()
{
    removeFlags(STATE_FLAG_TO_BE_SHOWN);
    m_isAnimationDone = false;
    if (isSet(STATE_FLAG_SHOWING)) //if it's not already hiding
        addFlags(STATE_FLAG_TO_BE_HIDDEN);
}
