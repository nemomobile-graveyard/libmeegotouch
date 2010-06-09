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

#ifndef APPLICATIONMENUPAGE_H
#define APPLICATIONMENUPAGE_H

#include "templatepage.h"
#include <MLabel>
class MGridLayoutPolicy;
class MLinearLayoutPolicy;
class MComboBox;

class ApplicationMenuPage : public TemplatePage
{
    Q_OBJECT

public:
    ApplicationMenuPage();
    virtual ~ApplicationMenuPage();
    virtual QString timedemoTitle();
    virtual void createContent();

public Q_SLOTS:
    void makeTextItalic();
    void makeTextNormal();
    void toggleWindowIconVisibility();

protected:
    virtual void retranslateUi();

private:
    int m_textIndex;
    MLinearLayoutPolicy  *policy;
    MComboBox *comboBox;
    MAction *actionItalic;
    MAction *actionNormal;
    MAction *actionIcon;

    void showText(int index);
};

#endif // APPLICATIONMENUPAGE_H
