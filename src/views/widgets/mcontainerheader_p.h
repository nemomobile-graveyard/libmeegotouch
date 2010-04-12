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

#include <MWidgetModel>
#include <MWidgetView>
#include "mcontainerheaderstyle.h"

class MContainerHeader;

/*!
 * \internal
 */

class MContainerHeaderView : public MWidgetView
{
    Q_OBJECT
    M_VIEW(MWidgetModel, MContainerHeaderStyle)

public:
    MContainerHeaderView(MContainerHeader *controller);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(MCancelEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);

signals:
    void pressed();
    void released();
    void canceled();
    void moved();
};

/*!
 * \internal
 */

class MContainerHeader : public MWidgetController
{
    Q_OBJECT
    M_CONTROLLER(MWidget) // uses MWidgetModel

public:
    MContainerHeader();
    virtual ~MContainerHeader() {}

signals:
    void pressed();
    void released();
    void canceled();
    void moved();
};
