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

#include "mextensionhandle.h"
#include "mapplicationextensionhandleview.h"
#include "mapplicationextensionhandleview_p.h"
#include <MMessageBox>
#include <MSceneManager>
#include <MAction>

MApplicationExtensionHandleViewPrivate::MApplicationExtensionHandleViewPrivate(MExtensionHandle *handle) :
    MExtensionHandleViewPrivate(handle)
{
}

MApplicationExtensionHandleViewPrivate::~MApplicationExtensionHandleViewPrivate()
{
}

void MApplicationExtensionHandleViewPrivate::showBrokenDialog()
{
    MMessageBox mb("This application extension is not functioning properly. Please restart the extension.", M::NoStandardButton);
    MButtonModel *restartButton = mb.addButton("Restart");

    controller->sceneManager()->execDialog(&mb);

    if (mb.clickedButton() == restartButton) {
        handle->reinit();
    }
}

void MApplicationExtensionHandleViewPrivate::showInstallationFailedDialog(const QString &error)
{
    MMessageBox mb("This application extension failed to install properly with the following error message: " + error + "Please remove the extension.", M::NoStandardButton);
    MButtonModel *removeButton = mb.addButton("Remove");

    controller->sceneManager()->execDialog(&mb);

    if (mb.clickedButton() == removeButton) {
        // TODO this doesn't really do anything yet
    }
}

void MApplicationExtensionHandleViewPrivate::drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool) const
{
    // Draw the extension pixmap scaled so that it fits the screen
    painter->drawPixmap(targetGeometry, localPixmap, sourceGeometry);
}

MApplicationExtensionHandleView::MApplicationExtensionHandleView(MExtensionHandle *handle) :
    MExtensionHandleView(*new MApplicationExtensionHandleViewPrivate(handle), handle)
{
}

MApplicationExtensionHandleView::MApplicationExtensionHandleView(MApplicationExtensionHandleViewPrivate &dd, MExtensionHandle *handle) :
    MExtensionHandleView(dd, handle)
{
}

MApplicationExtensionHandleView::~MApplicationExtensionHandleView()
{
}

M_REGISTER_VIEW_NEW(MApplicationExtensionHandleView, MExtensionHandle)
