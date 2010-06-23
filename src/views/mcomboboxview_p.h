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

class MLayout;
class MGridLayoutPolicy;
class MComboBox;
class MPopupList;
class MProgressIndicator;
class MLabel;
class MImageWidget;

#include "mlistitem.h"

class MComboBoxButton : public MListItem
{
public:
    MComboBoxButton(QGraphicsItem *parent = 0);

    void setIconID(const QString& id);
    void setTitle(const QString &title);
    void setSubtitle(const QString &subtitle);
    void setProgressIndicatorVisible(bool visible);
    void setIconVisible(bool visible);

    MImageWidget * iconWidget();
    MLabel * titleWidget();
    MLabel * subtitleWidget();
    MImageWidget * indicatorWidget();
    MProgressIndicator * progressWidget();

private:
    void initLayout();
    void updatePolicy();

    MGridLayoutPolicy* iconSubtitlePolicy();
    MGridLayoutPolicy* basicSubtitlePolicy();
    MGridLayoutPolicy* iconProgressPolicy();
    MGridLayoutPolicy* basicProgressPolicy();


    MLayout *layoutGrid;
    MGridLayoutPolicy* _iconSubtitlePolicy;
    MGridLayoutPolicy* _basicSubtitlePolicy;
    MGridLayoutPolicy* _iconProgressPolicy;
    MGridLayoutPolicy* _basicProgressPolicy;

    MImageWidget* _iconWidget;
    MLabel* _titleWidget;
    MLabel* _subtitleWidget;
    MImageWidget* _indicatorWidget;
    MProgressIndicator* _progressWidget;

    bool progressVisible;
    bool iconVisible;

#ifdef UNIT_TEST
    friend class Ut_MComboBox;
#endif
};

class MComboBoxViewPrivate
{
    Q_DECLARE_PUBLIC(MComboBoxView)

protected:
    MComboBoxView *q_ptr;

public:
    MComboBoxViewPrivate();
    virtual ~MComboBoxViewPrivate();

    void init();
    void updateSubtitle(int currentIndex);

    void _q_showPopup();
    void _q_dismissPopup();
    void _q_popuplistAppeared();
    void _q_popuplistDisappeared();
    void _q_itemModelCurrentIndexChanged(int currentIndex);

    MComboBox    *controller;
    MComboBoxButton *button;
    MPopupList   *popuplist;
    bool isPopupShowing;
};

#endif
