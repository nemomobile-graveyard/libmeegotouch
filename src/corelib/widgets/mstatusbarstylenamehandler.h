/***************************************************************************
**
** Copyright (C) 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef MSTATUSBARSTYLENAMEHANDLER_H
#define MSTATUSBARSTYLENAMEHANDLER_H

#include <QString>
#include <QWeakPointer>

class MSceneWindow;
class MSceneWindowEvent;

//! \internal

/*
  Sets the style name of the provided status bar depending
  on what scene windows come and go.

  The default MStatusBar style usually have a reactive margin on its bottom to
  make it easier to tap on it and bring the status menu. But while a sheet is being
  displayed this extra margin might partially cover the buttons on the sheet header
  (or at least be very close to them), causing the user to accidentally open the
  status menu when he wanted to press some sheet header button.

  This class assigns a different style name for the status bar (which will normaly
  have no reactive margins) whenever a sheet is the top-most scene window.
 */
class MStatusBarStyleNameHandler {
public:
    MStatusBarStyleNameHandler();
    void setStatusBar(MSceneWindow *statusBar);
    void setRegularStyleName(const QString &regularStyleName);
    void setStyleNameForSheets(const QString &styleNameForSheets);

    void sceneWindowAppearEvent(MSceneWindowEvent *ev);
    void sceneWindowDisappearEvent(MSceneWindowEvent *ev);

private:
    void updateStatusBarStyleName();

    QWeakPointer<MSceneWindow> statusBarPointer;
    int sheetsBeingShown;
    int sceneWindowsBlockingSheets;
    QString regularStyleName;
    QString styleNameForSheets;
};

//! \internal_end
#endif // MSTATUSBARSTYLENAMEHANDLER_H