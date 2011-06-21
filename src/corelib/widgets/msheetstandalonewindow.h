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
    void setSheet(MSceneWindow *sheet);

protected:
    virtual void closeEvent(QCloseEvent *event);
    virtual void showEvent(QShowEvent *event);
    virtual void hideEvent(QHideEvent *event);

private:
#ifdef Q_WS_X11
    void appendMSheetTypeProperty();
#endif //Q_WS_X11
    bool beingClosed;
    MSceneWindow *sheet;
};

//! \internal_end

#endif