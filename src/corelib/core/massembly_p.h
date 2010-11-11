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

#ifndef MASSEMBLY_P_H
#define MASSEMBLY_P_H

#include <QHash>
#include <QStringList>
#include "mstylesheet.h"

class MWidgetController;
class MLogicalValues;

class MAssemblyPrivate
{
public:

    QString name;
    QHash<QString, QString> viewConfiguration;
    mutable MStyleSheet *stylesheet;
    mutable bool styleSheetsLoaded;
    QStringList themeInheritance;

    bool loadViewConfiguration(const QStringList &themeInheritance);
    void loadStylesheet(const QString &stylesheet, const MLogicalValues &logicalValues) const;
    void loadStylesheets() const;
};

#endif
