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

#ifndef DUIGRIDPAGE_H
#define DUIGRIDPAGE_H

#include <MList>
#include <MVideoWidget>

#include "templatepage.h"
#include "gridmodel.h"

class MSlider;
class MLabel;

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

public slots:
    void videoReady();
    void itemClicked();

    void showGridConfigurationDialog();

    void modifyColumnsSliderHandle(int newValue);
    void modifyRowsSliderHandle(int newValue);

    void orientationChanged(M::Orientation orientation);

signals:
    void rate(MediaType::Rating rating, const QString& id);

protected:
    void retranslateUi();
    virtual void orientationChangeEvent(MOrientationChangeEvent *event);

private slots:
    void backButtonClicked();
    void configureGrid();
    void configureGrid(M::Orientation orientation);

private:
    MList *list;
    bool pageShown;
    QSizeF m_itemSize;

    int m_columnsPortrait;
    int m_columnsLandscape;

    MSlider* m_columnsLandscapeSlider;
    MSlider* m_columnsPortraitSlider;
    MLabel* m_columnsLandscapeLabel;
    MLabel* m_columnsPortraitLabel;
};

#endif
