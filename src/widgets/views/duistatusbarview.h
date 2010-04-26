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

#ifndef DUISTATUSBARVIEW_H
#define DUISTATUSBARVIEW_H

#include <views/duiscenewindowview.h>
#include <duiscenewindowmodel.h>


class DuiStatusBar;
class QPixmap;

//! \internal

class DuiStatusBarView : public DuiSceneWindowView
{
    Q_OBJECT
    DUI_VIEW(DuiSceneWindowModel, DuiSceneWindowStyle)

public:
    DuiStatusBarView(DuiStatusBar *controller);
    virtual ~DuiStatusBarView();

protected:
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

private:
    Q_DISABLE_COPY(DuiStatusBarView)

    DuiWidgetController *controller;

#ifdef Q_WS_X11
    void updateSharedPixmap();
    bool fetchSharedPixmapHandle(Qt::HANDLE *handle);
    void setupXDamageForSharedPixmap();
    void destroyXDamageForSharedPixmap();

    QPixmap sharedPixmap;
    Qt::HANDLE pixmapDamage;

private Q_SLOTS:
    void handlePixmapDamageEvent(Qt::HANDLE &damage, short &x, short &y,
                                 unsigned short &width, unsigned short &height);
#endif

};

//! \internal_end

#endif
