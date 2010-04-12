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

#include <MImageWidget>
#include <QString>
#include "gridmodel.h"

class MGridPage;

class GridImageWidget : public MImageWidget
{
    Q_OBJECT

public:
    GridImageWidget(QGraphicsItem *parent = 0);
    void setId(const QString& id);
    QString id();

Q_SIGNALS:
    void clicked();
    void rate(MediaType::Rating rating, const QString& id);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);

private:
    QString m_id;
    MGridPage* m_page;
};
