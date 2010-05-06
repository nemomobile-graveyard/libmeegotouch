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
    // the following use of translatable strings is not good.
    // Splitting sentences like this makes it hard to translate them
    // into language where the structure of the sentence is very
    // different from English.
    QString s;
    QTextStream(&s) <<
                    "<font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "The extension"
                    qtTrId("mapplicationextensionhandle_brokendialog_messagestart") <<
                    //~ uispec-document ??? FIXME
                    //% "is not functioning properly.<br />"
                    //% "Please restart the extension."
                    qtTrId("mapplicationextensionhandle_brokendialog_messageend") << "</font>";

    MMessageBox mb(s, M::NoStandardButton);
    //~ uispec-document ??? FIXME
    //% "Restart"
    MButtonModel *restartButton = mb.addButton(qtTrId("mapplicationextensionhandle_brokendialog_restart"));

    controller->sceneManager()->execDialog(&mb);

    if (mb.clickedButton() == restartButton) {
        handle->reinit();
    }
}

void MApplicationExtensionHandleViewPrivate::showInstallationFailedDialog(const QString &error)
{
    // the following use of translatable strings is not good.
    // Splitting sentences like this makes it hard to translate them
    // into language where the structure of the sentence is very
    // different from English.
    QString s;
    QTextStream(&s) <<
                    "<font color=\"white\">" <<
                    //~ uispec-document ??? FIXME
                    //% "The extension"
                    qtTrId("mapplicationextensionhandle_installationfaileddialog_messagestart") <<
                    //~ uispec-document ??? FIXME
                    //% "failed to install properly with the following error message: <br />"
                    qtTrId("mapplicationextensionhandle_installationfaileddialog_messagemiddle") <<
                    error <<
                    "<br />" <<
                    //% "Please remove the extension."
                    qtTrId("mapplicationextensionhandle_installationfaileddialog_messageend") << "</font>";

    MMessageBox mb(s, M::NoStandardButton);
    //~ uispec-document ??? FIXME
    //% "Remove"
    MButtonModel *removeButton = mb.addButton(qtTrId("mapplicationextensionhandle_brokendialog_remove"));

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
