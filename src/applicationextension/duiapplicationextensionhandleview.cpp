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

#include "duiextensionhandle.h"
#include "duiapplicationextensionhandleview.h"
#include "duiapplicationextensionhandleview_p.h"
#include <DuiMessageBox>
#include <DuiSceneManager>
#include <DuiAction>

DuiApplicationExtensionHandleViewPrivate::DuiApplicationExtensionHandleViewPrivate(DuiExtensionHandle *handle) :
    DuiExtensionHandleViewPrivate(handle)
{
}

DuiApplicationExtensionHandleViewPrivate::~DuiApplicationExtensionHandleViewPrivate()
{
}

void DuiApplicationExtensionHandleViewPrivate::showBrokenDialog()
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
                    qtTrId("duiapplicationextensionhandle_brokendialog_messagestart") <<
                    //~ uispec-document ??? FIXME
                    //% "is not functioning properly.<br />"
                    //% "Please restart the extension."
                    qtTrId("duiapplicationextensionhandle_brokendialog_messageend") << "</font>";

    DuiMessageBox mb(s, Dui::NoStandardButton);
    //~ uispec-document ??? FIXME
    //% "Restart"
    DuiButtonModel *restartButton = mb.addButton(qtTrId("duiapplicationextensionhandle_brokendialog_restart"));

    controller->sceneManager()->execDialog(&mb);

    if (mb.clickedButton() == restartButton) {
        handle->reinit();
    }
}

void DuiApplicationExtensionHandleViewPrivate::showInstallationFailedDialog(const QString &error)
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
                    qtTrId("duiapplicationextensionhandle_installationfaileddialog_messagestart") <<
                    //~ uispec-document ??? FIXME
                    //% "failed to install properly with the following error message: <br />"
                    qtTrId("duiapplicationextensionhandle_installationfaileddialog_messagemiddle") <<
                    error <<
                    "<br />" <<
                    //% "Please remove the extension."
                    qtTrId("duiapplicationextensionhandle_installationfaileddialog_messageend") << "</font>";

    DuiMessageBox mb(s, Dui::NoStandardButton);
    //~ uispec-document ??? FIXME
    //% "Remove"
    DuiButtonModel *removeButton = mb.addButton(qtTrId("duiapplicationextensionhandle_brokendialog_remove"));

    controller->sceneManager()->execDialog(&mb);
    if (mb.clickedButton() == removeButton) {
        // TODO this doesn't really do anything yet
    }
}

void DuiApplicationExtensionHandleViewPrivate::drawPixmap(QPainter *painter, const QRectF &sourceGeometry, const QRectF &targetGeometry, bool) const
{
    // Draw the extension pixmap scaled so that it fits the screen
    painter->drawPixmap(targetGeometry, localPixmap, sourceGeometry);
}

DuiApplicationExtensionHandleView::DuiApplicationExtensionHandleView(DuiExtensionHandle *handle) :
    DuiExtensionHandleView(*new DuiApplicationExtensionHandleViewPrivate(handle), handle)
{
}

DuiApplicationExtensionHandleView::DuiApplicationExtensionHandleView(DuiApplicationExtensionHandleViewPrivate &dd, DuiExtensionHandle *handle) :
    DuiExtensionHandleView(dd, handle)
{
}

DuiApplicationExtensionHandleView::~DuiApplicationExtensionHandleView()
{
}

DUI_REGISTER_VIEW_NEW(DuiApplicationExtensionHandleView, DuiExtensionHandle)
