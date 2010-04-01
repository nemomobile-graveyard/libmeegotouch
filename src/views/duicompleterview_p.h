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

#ifndef DUICOMPLETERVIEW_P_H
#define DUICOMPLETERVIEW_P_H

#include "duicompleter.h"
#include "duicompleterview.h"
#include "private/duiwidgetview_p.h"

class DuiCompleter;
class DuiWidgetController;
class DuiLabel;
class DuiButton;
class DuiPopupList;
class QStringListModel;
class QGraphicsLinearLayout;

//! \internal
class DuiCompleterViewPrivate : public QObject
{
    Q_OBJECT
    Q_DECLARE_PUBLIC(DuiCompleterView)

public:
    DuiCompleterViewPrivate(DuiCompleter *controller, DuiCompleterView *q);
    virtual ~DuiCompleterViewPrivate();

protected slots:

    void createContents();

    void clear();

    void showPopup();

    void organizeContents();

protected:
    void init();

    QPoint locatePosition(const QRect &r) const;

    //! The DuiCompleter controller
    DuiCompleter *controller;
    DuiCompleterView *q_ptr;

    DuiLabel *completionLabel;
    DuiButton *completionsButton;
    QGraphicsLinearLayout *layout;
    DuiPopupList *popup;
    QSizeF overLaypreferredSize;
};

#endif
