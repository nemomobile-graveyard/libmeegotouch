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

#ifndef MSHEETSTANDALONEWINDOW_H

//! \internal

#include <mwindow.h>

#include <QPointer>

#ifdef HAVE_CONTEXTSUBSCRIBER
# include "contextproperty.h"
#endif

class MSheet;
class MSceneWindow;

/*
  \class MSheetStandAloneWindow
  \brief For MSheet::appearSystemwide()
 */
class MSheetStandAloneWindow : public MWindow
{
    Q_OBJECT
public:
    MSheetStandAloneWindow();
    virtual ~MSheetStandAloneWindow();
    void setSheet(MSheet *sheet);

    void setStatusBarVisible(bool visible);
    void maximizeSheetArea();
    void restoreSheetArea();
    void updateChainTaskData();

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);

private:
#ifdef Q_WS_X11
    void appendMSheetTypeProperty();
    void setWindowChainedProperty(const WId &parentWinId, const WId &childWinId);
#endif //Q_WS_X11
    bool beingClosed;
    MSheet *sheet;
    QPointer<MSceneWindow> statusBar;
#ifdef HAVE_CONTEXTSUBSCRIBER
    ContextProperty callStatusProperty;
#endif
    bool sheetAreaMaximized;

private slots:
    void _q_updateStatusBarVisibility();
};

//! \internal_end

#endif
