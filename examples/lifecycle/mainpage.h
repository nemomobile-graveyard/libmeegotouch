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

#ifndef MAINPAGE_H
#define MAINPAGE_H

#include <MApplicationPage>
#include <QTimer>
#include <MProgressIndicator>
#include "anotherpage.h"
#include "containerpage.h"

class MainPage : public MApplicationPage
{
    Q_OBJECT

public:
    MainPage();
    virtual ~MainPage();
    virtual void createContent();

public slots:
    void buttonPress();
    void button2Press();
    void deactivateWidgets();
    void activateWidgets();
    void pauseWidgets();


private slots:
    void updateProgressBars();
    void indicatorVisible();
    void indicatorHidden();

protected:
    //! Visibility re-imps
    virtual void exitDisplayEvent();
    virtual void enterDisplayEvent();

private:
    MProgressIndicator *bar0;
    MProgressIndicator *bar1;
    AnotherPage *anotherPage;
    ContainerPage *canvasPage;
    QTimer *m_pTimer;
    int m_pbarValue;
};

#endif // MAINPAGE_H
