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

#include <DuiWidgetModel>
#include <DuiWidgetView>
#include "duicontainerheaderstyle.h"

class DuiContainerHeader;

/*!
 * \internal
 */

class DuiContainerHeaderView : public DuiWidgetView
{
    Q_OBJECT
    DUI_VIEW(DuiWidgetModel, DuiContainerHeaderStyle)

public:
    DuiContainerHeaderView(DuiContainerHeader *controller);

protected:
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void cancelEvent(DuiCancelEvent *event);
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

class DuiContainerHeader : public DuiWidgetController
{
    Q_OBJECT
    DUI_CONTROLLER(DuiWidget) // uses DuiWidgetModel

public:
    DuiContainerHeader();
    virtual ~DuiContainerHeader() {}

signals:
    void pressed();
    void released();
    void canceled();
    void moved();
};
