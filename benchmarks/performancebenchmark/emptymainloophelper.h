/***************************************************************************
**
** Copyright (C) 2010, 2011 Nokia Corporation and/or its subsidiary(-ies).
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

#ifndef EMPTYMAINLOOPHELPER_H
#define EMPTYMAINLOOPHELPER_H

#include <QObject>

/**
  * \class EmptyMainLoopHelper
  * \brief Allows to terminate the application when the main loop is empty.
  */
class EmptyMainLoopHelper : public QObject
{
    Q_OBJECT
public:
    enum TerminationType { ExitOnEmpty, QuitOnEmpty };

    EmptyMainLoopHelper();

    /**
      * Once this method is called the application is terminated as soon as the main loop is empty.
      * /param Defines how the application will be terminated.
      */
    void triggerTermination(TerminationType type);

public slots:
    /**
      * Exit the application if main loop is empty in combination with terminateOnEmptyMainLoop2().
      */
    void terminateOnEmptyMainLoop();

    /**
      * Called by terminateOnEmptyMainLoop() to terminate app if main loop is really empty.
      */
    void terminateOnEmptyMainLoop2();

    void quit();

private:
    TerminationType terminationType;
    bool readyToQuit;
};

#endif // EMPTYMAINLOOPHELPER_H
