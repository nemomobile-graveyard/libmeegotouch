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

#ifndef MLOGICALVALUES_H
#define MLOGICALVALUES_H

#include <QColor>
#include <QFont>

class MLogicalValuesPrivate;

//! \internal
class MLogicalValues
{
public:
    MLogicalValues();
    ~MLogicalValues();

    void load(const QStringList &themeInheritanceChain, const QString &locale = QString());
    bool append(const QString &fileName);

    bool findKey(const QByteArray &key, QByteArray &group, QByteArray &value) const;
    bool value(const QByteArray &group, const QByteArray &key, QByteArray &value) const;
    QColor color(const QByteArray &group, const QByteArray &key) const;
    QFont font(const QByteArray &group, const QByteArray &key) const;
    QList<uint> timestamps() const;

protected:
    MLogicalValuesPrivate *const d_ptr;

private:
    Q_DISABLE_COPY(MLogicalValues)
    Q_DECLARE_PRIVATE(MLogicalValues)
};
//! \internal_end

#endif
