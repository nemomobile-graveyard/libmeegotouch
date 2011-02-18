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

#ifndef MCOMPLETERVIEW_P_H
#define MCOMPLETERVIEW_P_H

#include "mcompleter.h"
#include "mcompleterview.h"
#include "private/mwidgetview_p.h"

class MCompleter;
class MWidgetController;
class MLabel;
class MButton;
class MPopupList;
class QStringListModel;
class QGraphicsLinearLayout;

//! \internal
class MCompleterViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(MCompleterView)

public:
    MCompleterViewPrivate(MCompleter *controller, MCompleterView *q);
    virtual ~MCompleterViewPrivate();

protected slots:

    void createContents();

    void clear();

    void showPopup();

    void refocusPopup();

    void organizeContents();

    void handlePopupAppearing();

    void handlePopupDisappeared();

    void handleCompleterShown();

    void handleCompleterHidden();

protected:
    void init();

    QPoint locatePosition(const QRect &r) const;

    //! The MCompleter controller
    MCompleter *controller;
    MCompleterView *q_ptr;

    MLabel *completionLabel;
    MButton *completionsButton;
    QGraphicsLinearLayout *layout;
    MPopupList *popup;
    QSizeF overLaypreferredSize;
};

#endif
