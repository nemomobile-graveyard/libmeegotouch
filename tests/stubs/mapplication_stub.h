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

#ifndef MAPPLICATION_STUB
#define MAPPLICATION_STUB

#include "mapplication.h"
#include <QApplication>
#include <stubbase.h>


class MFeedbackPlayer;

// 1. DECLARE STUB
// FIXME - stubgen is not yet finished
class MApplicationStub : public StubBase
{
public:
    virtual void MApplicationConstructor(int &argc, char **argv);
    virtual void MApplicationDestructor();
    virtual bool softwareRendering();
    virtual bool fullScreen();
    virtual bool showBoundingRect();
    virtual bool showFps();
    virtual bool showSize();
    virtual bool showPosition();
    virtual MApplicationWindow *applicationWindow();
    virtual void setApplicationWindow(MApplicationWindow *);
    virtual MFeedbackPlayer *feedbackPlayer();

};

// 2. IMPLEMENT STUB
void MApplicationStub::MApplicationConstructor(int &argc, char **argv)
{
    Q_UNUSED(argc);
    Q_UNUSED(argv);

}
void MApplicationStub::MApplicationDestructor()
{

}
bool MApplicationStub::softwareRendering()
{
    /*
      stubMethodEntered("softwareRendering");
      return stubReturnValue<bool>("softwareRendering");
    */
    return false;
}

bool MApplicationStub::fullScreen()
{
    /*
      stubMethodEntered("fullScreen");
      return stubReturnValue<bool>("fullScreen");
    */
    return false;
}

bool MApplicationStub::showBoundingRect()
{
    /*
      stubMethodEntered("showBoundingRect");
      return stubReturnValue<bool>("showBoundingRect");
    */
    return false;
}

bool MApplicationStub::showFps()
{
    /*
      stubMethodEntered("showFps");
      return stubReturnValue<bool>("showFps");
    */
    return false;
}

bool MApplicationStub::showSize()
{
    /*
      stubMethodEntered("showSize");
      return stubReturnValue<bool>("showSize");
    */
    return false;
}

bool MApplicationStub::showPosition()
{
    /*
      stubMethodEntered("showPosition");
      return stubReturnValue<bool>("showPosition");
    */
    return false;
}

MApplicationWindow *MApplicationStub::applicationWindow()
{
    stubMethodEntered("applicationWindow");
    return stubReturnValue<MApplicationWindow *>("applicationWindow");
}

void MApplicationStub::setApplicationWindow(MApplicationWindow *appWindow)
{
    QList<ParameterBase *> params;
    params.append(new Parameter<MApplicationWindow * >(appWindow));
    stubMethodEntered("setApplicationWindow", params);
}

MFeedbackPlayer *MApplicationStub::feedbackPlayer()
{
    stubMethodEntered("feedbackPlayer");
    return (MFeedbackPlayer *) NULL;
}


// 3. CREATE A STUB INSTANCE
MApplicationStub gDefaultMApplicationStub;
MApplicationStub *gMApplicationStub = &gDefaultMApplicationStub;


// 4. CREATE A PROXY WHICH CALLS THE STUB
//

MFeedbackPlayer *MApplication::feedbackPlayer()
{
    return gMApplicationStub->feedbackPlayer();
}

MApplication::MApplication(int &argc, char **argv)
    : QApplication(argc, argv), d_ptr(0)
{
    gMApplicationStub->MApplicationConstructor(argc, argv);
}

MApplication::~MApplication()
{
    gMApplicationStub->MApplicationDestructor();
}

bool MApplication::softwareRendering()
{
    return gMApplicationStub->softwareRendering();
}

bool MApplication::fullScreen()
{
    return gMApplicationStub->fullScreen();
}

bool MApplication::showBoundingRect()
{
    return gMApplicationStub->showBoundingRect();
}

bool MApplication::showSize()
{
    return gMApplicationStub->showSize();
}


bool MApplication::showPosition()
{
    return gMApplicationStub->showPosition();
}

bool MApplication::showFps()
{
    return gMApplicationStub->showFps();
}


MApplicationWindow *MApplication::applicationWindow()
{
    return gMApplicationStub->applicationWindow();
}

void MApplication::setApplicationWindow(MApplicationWindow *appWin)
{
    gMApplicationStub->setApplicationWindow(appWin);
}

#endif
