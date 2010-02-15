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

#ifndef WIDGETSGALLERY_TIMINGSCENE_H
#define WIDGETSGALLERY_TIMINGSCENE_H

#include <DuiScene>

/* A custom DuiScene for timing the framerate.
 */
class TimingScene : public DuiScene
{
    Q_OBJECT

public:
    explicit TimingScene(QObject *parent = 0);
    virtual ~TimingScene();

    void setUpdateContinuously(bool value) {
        m_updateContinuously = value;
    }

    int frameCount() const {
        return m_frameCount;
    }

protected:
    virtual void drawForeground(QPainter *painter, const QRectF &rect);

private:
    bool m_updateContinuously;
    int m_frameCount;
};

#endif
