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
    m_translateProgress(1),
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

QPointF MItemState::sourceTranslate() const
{
    return m_sourceTranslate;
}

MItemState::Flags MItemState::flags() const
{
    return m_flags;
}

void MItemState::setSourceTranslatePoint(const QPointF &sourceTranslate)
{
    Q_ASSERT(m_item);
    if(m_sourceTranslate == sourceTranslate)
        return;
    m_sourceTranslate = sourceTranslate;
    m_isAnimationDone = false;
    if (!m_sourceTranslate.isNull())
        m_translateProgress = 0;
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
    m_translateProgress = 1;
    m_sourceTranslate = QPointF(0,0);
    m_sourceOpacity = m_targetOpacity = -1;
    //Keep the deleted flag so that it can be deleted
    if (m_flags & STATE_FLAG_TO_BE_HIDDEN) {
        m_flags &= ~STATE_FLAG_TO_BE_HIDDEN;
        m_flags &= ~STATE_FLAG_SHOWING;
    }
}

void MItemState::setTransformProgress(qreal progress)
{
    m_translateProgress = progress;
    if (progress != 1)
        m_isAnimationDone = false;
}
void MItemState::setOpacityProgress(qreal progress)
{
    m_opacityProgress = progress;
    if (progress != 1)
        m_isAnimationDone = false;
}
qreal MItemState::translateProgress() const
{
    return m_translateProgress;
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
    m_isAnimationDone = false;
    if (isSet(STATE_FLAG_SHOWING)) //if it's not already hiding
        addFlags(STATE_FLAG_TO_BE_HIDDEN);
}
