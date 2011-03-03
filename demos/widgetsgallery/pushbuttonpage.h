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

#ifndef PUSHBUTTONPAGE_H
#define PUSHBUTTONPAGE_H

#include "mainpage.h"
#include "templatepage.h"
#include <MApplicationPage>
#include <MLayout>
#include <MGridLayoutPolicy>

class MButton ;
class MWidget;
class MLabel;

class PushButtonPage : public TemplatePage
{
    Q_OBJECT
public:
    PushButtonPage();
    virtual ~PushButtonPage();
    virtual QString timedemoTitle();
    virtual void createContent();

protected:
    virtual void retranslateUi();

private:
    void singleButton();
    void twoButtons();
    void threeButtons();
    void twoButtonsRowLayout();
    void threeButtonsRowLayout();
    void callRejectButtonLayout();
    void callRejectRowLayout();
    void splitButtonsLayout();
    void buttonTransitionsLayout();

    MLinearLayoutPolicy *layoutPolicy;

    MButton *button1;
    MButton *button2;
    MButton *button3;
    MButton *button4;
    MButton *button5;
    MButton *button6;
    MButton *button7;
    MButton *button8;
    MButton *button9;
    MButton *button10;
    MButton *button11;
    MButton *button12;
    MButton *button13;
    MButton *button14;
    MButton *button15;
    MButton *button16;
    MButton *button17;
    MButton *button18;
    MButton *button19;
    MButton *button20;
    MButton *button21;
    MButton *button22;
    MButton *button23;
    MButton *button24;
    MButton *button25;

    MLabel *text1;
    MLabel *text2;
    MLabel *text3;
    MLabel *text4;
    MLabel *text5;
    MLabel *text6;
    MLabel *text7;
    MLabel *text8;
    MLabel *text9;
    MLabel *text10;
    MLabel *text11;
    MLabel *text12;
    MLabel *text13;
    MLabel *text14;
};

#endif
