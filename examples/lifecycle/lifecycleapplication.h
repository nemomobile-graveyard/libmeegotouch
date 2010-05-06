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

#ifndef LIFECYCLEAPPLICATION_H
#define LIFECYCLEAPPLICATION_H

#include <QObject>
#include <MApplication>

class LifeCycleApplication : public MApplication
{
    Q_OBJECT

public:

    bool m_bHandleSignal;

    LifeCycleApplication(int &argc, char **argv, const QString &appIdentifier, MApplicationService *service);

    virtual ~LifeCycleApplication();
    virtual void releaseMemory();

private:

    void showReleaseMemory(QString message);

private slots:
    void releaseMemoryHandler();

};
#endif // LIFECYCLEAPPLICATION_H
