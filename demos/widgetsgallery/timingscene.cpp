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

#include "timingscene.h"

#include <DuiApplication>
#include <QTimer>

TimingScene::TimingScene(QObject *parent)
    :
    DuiScene(parent),
    m_updateContinuously(false),
    m_frameCount(0)
{}

TimingScene::~TimingScene()
{}

void TimingScene::setUpdateContinuously(bool value)
{
    m_updateContinuously = value;
    if (value) {
        // trigger an update to enable continuous redrawing
        update();
    }
}

void TimingScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    DuiScene::drawForeground(painter, rect);

    ++m_frameCount;

    if (m_updateContinuously && !DuiApplication::showFps()) {
        // Update again on return to the main loop, in order to refresh
        // the scene as frequently as possible for benchmark purposes.
        QTimer::singleShot(0, this, SLOT(update()));
    }
}
