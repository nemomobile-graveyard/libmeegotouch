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

#ifndef MGRIDPAGE_H
#define MGRIDPAGE_H

#include <MList>

#include "templatepage.h"
#include "gridmodel.h"

class MSlider;
class MLabel;
class MSheet;

class MGridPage: public TemplatePage
{
    Q_OBJECT

public:
    MGridPage();
    virtual ~MGridPage();

    virtual void createContent();
    virtual QString timedemoTitle();

    QSizeF itemSize() const
    {
        return m_itemSize;
    }

public Q_SLOTS:
    void itemClicked(const QModelIndex &index);

    void showGridConfigurationSheet();
    void modifyColumnsSliderHandle(int newValue);
    void modifyRowsSliderHandle(int newValue);
    void configurationUpdated();
    void configurationCanceled();

signals:
protected:
    void retranslateUi();
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);

private slots:
    void configureGrid();
    void configureGrid(M::Orientation orientation);

    void pauseLoaders();
    void resumeLoaders();

private:
    MList *list;
    GridModel *model;
    MAction *actionConfiguration;
    QSizeF m_itemSize;

    int m_columnsPortrait;
    int m_columnsLandscape;

    MSlider* m_columnsLandscapeSlider;
    MSlider* m_columnsPortraitSlider;

    MSheet* configurationSheet;
};

#endif
