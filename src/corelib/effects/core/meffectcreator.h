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

#ifndef MEFFECTCREATOR_H
#define MEFFECTCREATOR_H

#include "mexport.h"

#define M_REGISTER_EFFECT(EFFECT) \
static const MEffectCreator<EFFECT> g_EffectCreator(#EFFECT);

// forward declarations
class QGraphicsEffect;

/*!
   Interface for MEffectCreators
   you can implement your own creator or use MEffectCreator template class with
   M_REGISTER_EFFECT-macro.
 */
class M_CORE_EXPORT MEffectCreatorBase
{
public:
    /*!
       Constructor
       Registers this creator instance to MClassFactory.
     */
    MEffectCreatorBase(const char *effectClassName);

    /*!
       Destructor
       Unregisters this creator instance from MClassFactory.
     */
    virtual ~MEffectCreatorBase();

    /*!
       Returns a new graphics effect instance.
       Ownership is transferred to caller.
     */
    virtual QGraphicsEffect *create() const = 0;
};

template<class EFFECT>
class MEffectCreator : public MEffectCreatorBase
{
public:
    MEffectCreator(const char *effectClassName) :
        MEffectCreatorBase(effectClassName)
    {}
    virtual ~MEffectCreator()
    {}

    virtual QGraphicsEffect *create() const {
        return new EFFECT();
    }
};

#endif // MEFFECTCREATOR_H
