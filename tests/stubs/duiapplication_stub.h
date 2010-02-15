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

#ifndef DUIAPPLICATION_STUB
#define DUIAPPLICATION_STUB

#include "duiapplication.h"
#include <QApplication>
#include <stubbase.h>


class DuiFeedbackPlayer;

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class DuiApplicationStub : public StubBase
{
public:
    virtual void DuiApplicationConstructor(int &argc, char **argv);
    virtual void DuiApplicationDestructor();
    virtual bool softwareRendering();
    virtual bool fullScreen();
    virtual bool showBoundingRect();
    virtual bool showFps();
    virtual bool showSize();
    virtual bool showPosition();
    virtual DuiApplicationWindow *applicationWindow();
    virtual void setApplicationWindow(DuiApplicationWindow *);
    virtual DuiFeedbackPlayer *feedbackPlayer();

};

// 2. IMPLEMENT STUB
void DuiApplicationStub::DuiApplicationConstructor(int &argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

}
void DuiApplicationStub::DuiApplicationDestructor()
{

}
bool DuiApplicationStub::softwareRendering()
{
    /*
      stubMethodEntered("softwareRendering");
      return stubReturnValue<bool>("softwareRendering");
    */
    return false;
}

bool DuiApplicationStub::fullScreen()
{
    /*
      stubMethodEntered("fullScreen");
      return stubReturnValue<bool>("fullScreen");
    */
    return false;
}

bool DuiApplicationStub::showBoundingRect()
{
    /*
      stubMethodEntered("showBoundingRect");
      return stubReturnValue<bool>("showBoundingRect");
    */
    return false;
}

bool DuiApplicationStub::showFps()
{
    /*
      stubMethodEntered("showFps");
      return stubReturnValue<bool>("showFps");
    */
    return false;
}

bool DuiApplicationStub::showSize()
{
    /*
      stubMethodEntered("showSize");
      return stubReturnValue<bool>("showSize");
    */
    return false;
}

bool DuiApplicationStub::showPosition()
{
    /*
      stubMethodEntered("showPosition");
      return stubReturnValue<bool>("showPosition");
    */
    return false;
}

DuiApplicationWindow *DuiApplicationStub::applicationWindow()
{
    stubMethodEntered("applicationWindow");
    return stubReturnValue<DuiApplicationWindow *>("applicationWindow");
}

void DuiApplicationStub::setApplicationWindow(DuiApplicationWindow *appWindow)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<DuiApplicationWindow * >(appWindow));
    stubMethodEntered("setApplicationWindow", params);
}

DuiFeedbackPlayer *DuiApplicationStub::feedbackPlayer()
{
    stubMethodEntered("feedbackPlayer");
    return (DuiFeedbackPlayer *) NULL;
}


// 3. CREATE A STUB INSTANCE
DuiApplicationStub gDefaultDuiApplicationStub;
DuiApplicationStub *gDuiApplicationStub = &gDefaultDuiApplicationStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
//

DuiFeedbackPlayer *DuiApplication::feedbackPlayer()
{
    return gDuiApplicationStub->feedbackPlayer();
}

DuiApplication::DuiApplication(int &argc, char **argv)
    : QApplication(argc, argv), d_ptr(0)
{
    gDuiApplicationStub->DuiApplicationConstructor(argc, argv);
}

DuiApplication::~DuiApplication()
{
    gDuiApplicationStub->DuiApplicationDestructor();
}

bool DuiApplication::softwareRendering()
{
    return gDuiApplicationStub->softwareRendering();
}

bool DuiApplication::fullScreen()
{
    return gDuiApplicationStub->fullScreen();
}

bool DuiApplication::showBoundingRect()
{
    return gDuiApplicationStub->showBoundingRect();
}

bool DuiApplication::showSize()
{
    return gDuiApplicationStub->showSize();
}


bool DuiApplication::showPosition()
{
    return gDuiApplicationStub->showPosition();
}

bool DuiApplication::showFps()
{
    return gDuiApplicationStub->showFps();
}


DuiApplicationWindow *DuiApplication::applicationWindow()
{
    return gDuiApplicationStub->applicationWindow();
}

void DuiApplication::setApplicationWindow(DuiApplicationWindow *appWin)
{
    gDuiApplicationStub->setApplicationWindow(appWin);
}

#endif
