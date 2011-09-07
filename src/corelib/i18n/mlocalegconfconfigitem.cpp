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

#include "mlocalegconfconfigitem.h"
#include <mgconfitem.h>

class MLocaleGConfConfigItemPrivate {
  public:
    MLocaleGConfConfigItemPrivate( const QString& key ) : key( key ), gconfItem( key ) {}

    bool valid;
    QString key;
    QString value;
    MGConfItem gconfItem;
};

MLocaleGConfConfigItem::MLocaleGConfConfigItem( const QString& key ) :
    ML10N::MLocaleAbstractConfigItem( key ),
    d_ptr( new MLocaleGConfConfigItemPrivate( key ) )
{
    connect( &d_ptr->gconfItem, SIGNAL( valueChanged() ),
             this, SLOT( slotValueChanged() ) );
}

MLocaleGConfConfigItem::~MLocaleGConfConfigItem()
{
    delete d_ptr;
}

bool MLocaleGConfConfigItem::isValid() const
{
    return d_ptr->gconfItem.value().isValid();
}

QString MLocaleGConfConfigItem::value() const
{
    return d_ptr->gconfItem.value().toString();
}

void MLocaleGConfConfigItem::setValue( const QString& value )
{
    d_ptr->gconfItem.set( value );
    emit valueChanged( value );
}

void MLocaleGConfConfigItem::slotValueChanged()
{
    emit valueChanged( d_ptr->gconfItem.value().toString() );
}
