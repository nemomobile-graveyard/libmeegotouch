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

#ifndef DUINAVIGATIONBARVIEW_H
#define DUINAVIGATIONBARVIEW_H

#include "duiscenewindowview.h"
#include "duinavigationbarmodel.h"
#include "duinavigationbarstyle.h"

class DuiNavigationBarViewPrivate;
class DuiNavigationBar;

class DUI_EXPORT DuiNavigationBarView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiNavigationBarModel, DuiNavigationBarStyle)

public:
    DuiNavigationBarView(DuiNavigationBar *controller);
    virtual ~DuiNavigationBarView();


protected:
    //! \reimp
    virtual void setupModel();
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    //! \reimp_end

protected Q_SLOTS:
    //! \reimp
    virtual void updateData(const QList<const char *>& modifications);
    //! \reimp_end

private:
    Q_DISABLE_COPY(DuiNavigationBarView)
    Q_DECLARE_PRIVATE(DuiNavigationBarView)
};

#endif
