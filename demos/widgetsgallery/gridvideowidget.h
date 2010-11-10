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

#ifndef DUIGRIDVIDEOWIDGET_H
#define DUIGRIDVIDEOWIDGET_H

#ifdef HAVE_GSTREAMER

#include <MVideoWidget>

class QGraphicsSceneContextMenuEvent;
class MGridPage;

//video widget which emits clicked signal
class GridVideoWidget : public MVideoWidget
{
    Q_OBJECT

    public:
        GridVideoWidget(QGraphicsItem *parent = 0);
        void setId(const QString& id);
        QString id();

    Q_SIGNALS:
        void clicked();
        void longPressed();

    protected:

         virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
         virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
         virtual void contextMenuEvent(QGraphicsSceneContextMenuEvent *event);

    private:

        QString m_id;
};

#endif
#endif
