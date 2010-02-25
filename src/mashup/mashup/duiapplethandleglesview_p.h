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

#ifndef DUIAPPLETHANDLEGLESVIEW_P_H_
#define DUIAPPLETHANDLEGLESVIEW_P_H_

#include "duiapplethandleview_p.h"

class DuiAppletHandleGLESView;
class DuiAppletHandle;

class DuiAppletHandleGLESViewPrivate : public DuiAppletHandleViewPrivate
{
    Q_DECLARE_PUBLIC(DuiAppletHandleGLESView)

public:
    DuiAppletHandleGLESViewPrivate(DuiAppletHandle *appletHandle);
    virtual ~DuiAppletHandleGLESViewPrivate();

    //! OpenGL texture id for the applet view
    qint32 textureID;

    //! The applet pixmap with the broken state visuals applied
    QPixmap brokenAppletPixmap;

    //! Deletes the pixmaps
    virtual void destroyPixmaps();

    //! Draws graphics on top of the applet to show the broken state.
    virtual void drawBrokenState();

    //! \reimp
    virtual void drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool brokenState) const;
    //! \reimp_end
};

#endif /* DUIAPPLETHANDLEGLESVIEW_P_H_ */
