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

#ifndef PROGRESSBARPAGE_H
#define PROGRESSBARPAGE_H

#include "templatepage.h"
#include <QTimer>


class DuiGridLayoutPolicy;
class DuiProgressIndicator;
class DuiButton;
class DuiLabel;

class ProgressBarPage : public TemplatePage
{
    Q_OBJECT;
public:
    ProgressBarPage();
    virtual ~ProgressBarPage();
    virtual QString timedemoTitle();
    virtual void createContent();
public slots:
    void timeout();
protected:
    virtual void retranslateUi();
private:
    DuiProgressIndicator *bar1;
    DuiProgressIndicator *bar2;
    DuiLabel *label1;
    DuiLabel *label2;
    QTimer timer;
};

#endif // PROGRESSBARPAGE_H
