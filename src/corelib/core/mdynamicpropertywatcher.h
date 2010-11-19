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

#ifndef MDYNAMICPROPERTYWATCHER_P_H
#define MDYNAMICPROPERTYWATCHER_P_H

//! \internal

#include <QObject>
#include <QVariant>

/*
 Only tells that a dynamic property has changed when its actual value
 has changed.
 */
class MDynamicPropertyWatcher : public QObject
{
    Q_OBJECT
public:
    MDynamicPropertyWatcher(QObject* parent = 0);

    void watch(QObject *object);
    void setPropertyName(QString propertyName);

    bool eventFilter(QObject* watched, QEvent* event);

Q_SIGNALS:
    void propertyChanged();

private:
    QVariant lastPropertyValue;
    QObject *watchedObject;
    QString propertyName;
};

//! \internal_end

#endif