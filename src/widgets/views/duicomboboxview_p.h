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

#ifndef DUICOMBOBOXVIEW_P_H
#define DUICOMBOBOXVIEW_P_H

#include "private/duiwidgetview_p.h"
#include <QModelIndex>

class QGraphicsGridLayout;
class DuiComboBox;
class DuiImageWidget;
class DuiLabel;
class DuiPopupList;

class DuiComboBoxViewPrivate : public DuiWidgetViewPrivate
{
    Q_DECLARE_PUBLIC(DuiComboBoxView)

public:
    DuiComboBoxViewPrivate();
    virtual ~DuiComboBoxViewPrivate();

    void init();
    void initLayout();
    void _q_showPopup();
    void _q_itemModelCurrentIndexChanged(int currentIndex);

    DuiComboBox    *controller;
    QGraphicsGridLayout    *layout;
    DuiLabel       *title;
    DuiLabel       *subtitle;
    DuiImageWidget *icon;
    DuiPopupList   *popuplist;
};

#endif
