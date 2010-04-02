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

#ifndef MCOMBOBOXVIEW_P_H
#define MCOMBOBOXVIEW_P_H

class QGraphicsLinearLayout;
class MComboBox;
class MContentItem;
class MPopupList;
class MProgressIndicator;

class MComboBoxViewPrivate
{
    Q_DECLARE_PUBLIC(MComboBoxView)

protected:
    MComboBoxView *q_ptr;

public:
    MComboBoxViewPrivate();
    virtual ~MComboBoxViewPrivate();

    void init();
    void initLayout();
    void updateSubtitle(int currentIndex);

    void _q_showPopup();
    void _q_itemModelCurrentIndexChanged(int currentIndex);

    MComboBox    *controller;
    QGraphicsLinearLayout    *layout;
    MContentItem *contentItem;
    MPopupList   *popuplist;
    QPixmap *pixmap;
    MProgressIndicator *progressIndicator;
};

#endif
