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

#include <QPointer>

class MLayout;
class MLinearLayoutPolicy;
class MComboBox;
class MComboBoxButton;
class MPopupList;
class MLabel;

class MComboBoxViewPrivate
{
    Q_DECLARE_PUBLIC(MComboBoxView)

protected:
    MComboBoxView *q_ptr;

public:
    MComboBoxViewPrivate();
    virtual ~MComboBoxViewPrivate();

    void init();
    void updateTitle(const QString& title);
    void updateSubtitle(int currentIndex);

    MComboBoxButton* buttonWidget();
    MLabel* titleWidget();
    MLinearLayoutPolicy* separateTitlePolicy();
    MLinearLayoutPolicy* integratedTitlePolicy();

    void _q_showPopup();
    void _q_dismissPopup();
    void _q_popuplistAppeared();
    void _q_popuplistDisappeared();
    void _q_itemModelCurrentIndexChanged(int currentIndex);

    MComboBox    *controller;
    MComboBoxButton *button;
    QPointer<MPopupList> popuplist;
    MLabel *title;
    MLayout *layout;
    MLinearLayoutPolicy *separateTitle;
    MLinearLayoutPolicy *integratedTitle;

    bool isPopupShowing;
};

#endif
