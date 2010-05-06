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

#ifndef MAIN_H_
#define MAIN_H_

#include <QObject>
#include <QList>
#include <MApplicationPage>
#include <mapplicationextensioninterface.h>
#include <demoapplicationextensioninterface.h>

class MLabel;

class DemoPage : public MApplicationPage, public DemoApplicationInterface
{
    Q_OBJECT

public:
    DemoPage() : value(0) {}
    virtual void createContent();
    virtual void setDemoExtensionControlledValue(int value);
    virtual void setDemoExtensionControlledValue(const QString &value);

public slots:
    void addExtension(MApplicationExtensionInterface *extension);
    void removeExtension(MApplicationExtensionInterface *extension);
    void resetExtensions();

private:
    void updateLabel();

    QList<DemoApplicationExtensionInterface*> extensions;

    MLabel *extensionValueLabel;
    int value;
    QString valueText;
};

#endif
