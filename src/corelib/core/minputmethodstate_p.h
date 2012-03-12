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

#ifndef MINPUTMETHODSTATE_P_H
#define MINPUTMETHODSTATE_P_H

#include <QObject>
#include <QMap>
#include <QVariant>

#ifdef HAVE_MALIIT
#include <maliit/attributeextension.h>
#endif

#include "minputmethodstate.h"
#include "mnamespace.h"

class AttributeExtensionInfo : public QObject
{
    Q_OBJECT

public:
    explicit AttributeExtensionInfo(const QString &attributeExtensionFile);

#ifdef HAVE_MALIIT
    QScopedPointer<Maliit::AttributeExtension> extension;
#else
    QString fileName;
#endif
    MInputMethodState::ExtendedAttributeMap extendedAttributes;

#ifdef HAVE_MALIIT
public Q_SLOTS:
    void updateExtendedAttribute(const QString &key,
                                 const QVariant &value);

Q_SIGNALS:
    //! Emitted when input method extended attribute is changed.
    void extendedAttributeChanged(int id, const QString &target, const QString &targetItem,
                                  const QString &attribute, const QVariant &value);
#endif
};

class MInputMethodStatePrivate
{
    Q_DECLARE_PUBLIC(MInputMethodState)
public:
    MInputMethodStatePrivate();
    ~MInputMethodStatePrivate();

    void init();

    //! Current input method area
    QRect region;

    //! Current orientation angle of application's active window
    M::OrientationAngle orientationAngle;

    //! Indicate during rotation changing.
    bool rotationInProgress;

    QMap<int, AttributeExtensionInfo*> attributeExtensions;

private:
    Q_DISABLE_COPY(MInputMethodStatePrivate)

    MInputMethodState *q_ptr;
};

#endif
