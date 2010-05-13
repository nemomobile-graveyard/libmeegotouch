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

#ifndef MSTATUSBARVIEW_H
#define MSTATUSBARVIEW_H

#include <mscenewindowview.h>
#include <mscenewindowmodel.h>


class MStatusBar;
class QPixmap;

//! \internal

class MStatusBarView : public MSceneWindowView
{
    Q_OBJECT
    M_VIEW(MSceneWindowModel, MSceneWindowStyle)

public:
    MStatusBarView(MStatusBar *controller);
    virtual ~MStatusBarView();

protected:
    virtual void drawContents(QPainter *painter, const QStyleOptionGraphicsItem *option) const;

private:
    Q_DISABLE_COPY(MStatusBarView)

    MWidgetController *controller;
    bool updatesEnabled;

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

    void enablePixmapUpdates();
    void disablePixmapUpdates();

#endif

#ifdef UNIT_TEST
    friend class Ut_MStatusBarView;
#endif

};

//! \internal_end

#endif
