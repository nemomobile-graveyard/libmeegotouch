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

#ifndef CONTINUOUSSLIDERPAGE_H
#define CONTINUOUSSLIDERPAGE_H

#include "templatepage.h"

class DuiGridLayoutPolicy;
class DuiLabel;
class DuiSlider;
class DuiSeekBar;

class ContinuousSliderPage : public TemplatePage
{
    Q_OBJECT;

public:
    ContinuousSliderPage();
    virtual ~ContinuousSliderPage();

    virtual void createContent();

public slots:
    void rotate(int d);
    void rotatel();
    void rotater();

    void pageRotated(const Dui::Orientation &orientation);

protected:
    virtual void retranslateUi();
    DuiGridLayoutPolicy *l_policy;
    DuiGridLayoutPolicy *p_policy;

private:
    DuiLabel *label1;
    DuiLabel *label2;
    DuiLabel *label3;
    DuiSlider *slider3;
    DuiLabel *label4;
    DuiSeekBar *seekbar4;
};

#endif
