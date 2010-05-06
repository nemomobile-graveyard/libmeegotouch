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

#ifndef ANOTHERPAGE_H
#define ANOTHERPAGE_H

#include <MApplicationPage>
#include <MLabel>
#include <QStringList>
#include <QString>

const int LABEL_COUNT = 10;

class MGridLayoutPolicy;
class MProgressIndicator;
class MButton;

class AnotherPage : public MApplicationPage
{
    Q_OBJECT;

public:
    AnotherPage();
    virtual ~AnotherPage();

    virtual void createContent();

private slots:
    void clearLog();
    void itemVisible();
    void itemHidden();
    void windowVisible();
    void windowHidden();

protected:
    MGridLayoutPolicy *l_policy;
    MGridLayoutPolicy *p_policy;

    //! Visibility re-imps
    virtual void enterDisplayEvent();
    virtual void exitDisplayEvent();

private:
    void fillLogLabel(QString text);
    MLabel *logLabel;
    MLabel *panLabel;
    MLabel *windowVisibleLabel;
    MLabel *windowHiddenLabel;
    MLabel *labelList[LABEL_COUNT];
    QStringList labelBgColors;
    QStringList logLabelRows;
    int labelBgColorIndex;
    int hiddenCount;
};

#endif // ANOTHERPAGE_H
