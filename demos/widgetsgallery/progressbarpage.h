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

#ifndef PROGRESSBARPAGE_H
#define PROGRESSBARPAGE_H

#include "templatepage.h"
#include <QTimer>


class MGridLayoutPolicy;
class MProgressIndicator;
class MButton;
class MLabel;

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
    MProgressIndicator *bar1;
    MProgressIndicator *bar2;
    MLabel *label1;
    MLabel *label2;
    QTimer timer;
};

#endif // PROGRESSBARPAGE_H
