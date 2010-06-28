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

#ifndef MCOMBOBOXBUTTON_H
#define MCOMBOBOXBUTTON_H

#include "mlistitem.h"

class MImageWidget;
class MLabel;
class MProgressIndicator;
class MLayout;
class MGridLayoutPolicy;

//! \internal

class MComboBoxButton : public MListItem
{
public:
    MComboBoxButton(QGraphicsItem *parent = 0);

    void setIconID(const QString& id);
    void setTitle(const QString &title);
    void setSubtitle(const QString &subtitle);
    void setProgressIndicatorVisible(bool visible);
    void setIconVisible(bool visible);
    void setTitleVisible(bool visible);

    void setIconObjectName(const QString& name);
    void setTitleObjectName(const QString& name);
    void setSubTitleObjectName(const QString& name);
    void setProgressIndicatorObjectName(const QString& name);
    void setIndicatorObjectName(const QString& name);
    void setIndicatorImage(const QString& image);

private:
    void updateLayout();
    MGridLayoutPolicy* layoutPolicy(int flags);

    MImageWidget * iconWidget();
    MLabel * titleWidget();
    MLabel * subtitleWidget();
    MImageWidget * indicatorWidget();
    MProgressIndicator * progressWidget();

    MGridLayoutPolicy* policy[8];

    MImageWidget* icon;
    MLabel* title;
    MLabel* subtitle;
    MImageWidget* indicator;
    MProgressIndicator* progress;

    QString iconObjectName;
    QString titleObjectName;
    QString subTitleObjectName;
    QString progressIndicatorObjectName;
    QString indicatorObjectName;
    QString iconImageID;
    QString titleText;
    QString indicatorImage;

    enum ShownWidget {
        IconShown = 1,
        ProgressShown = 2,
        TitleShown = 4
    };
    int flags;

#ifdef UNIT_TEST
    friend class Ut_MComboBox;
#endif
};

//! \internal_end

#endif
