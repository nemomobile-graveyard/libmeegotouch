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

#ifndef MLOCALEGCONFCONFIGITEM_H
#define MLOCALEGCONFCONFIGITEM_H

#include <QString>

#include "mexport.h"
#include "mlocale/mlocaleabstractconfigitem.h"

class MLocaleGConfConfigItemPrivate;

class M_CORE_EXPORT MLocaleGConfConfigItem : public ML10N::MLocaleAbstractConfigItem
{
Q_OBJECT
  public:
    MLocaleGConfConfigItem( const QString& key );
    virtual ~MLocaleGConfConfigItem();

    bool isValid() const;
    QString value() const;
    void setValue( const QString& value );

  private:
    MLocaleGConfConfigItemPrivate *const d_ptr;

  private Q_SLOTS:
    void slotValueChanged();
};

#endif
