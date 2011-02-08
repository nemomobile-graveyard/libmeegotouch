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

#ifndef MANIMATIONCREATOR_H
#define MANIMATIONCREATOR_H

#include "mexport.h"

#define M_REGISTER_ANIMATION(ANIMATION) \
    static const MAnimationCreator<ANIMATION> g_AnimationCreator(#ANIMATION);

// forward declarations
class QAbstractAnimation;

/*!
   Interface for MAnimationCreators
   you can implement your own creator or use MAnimationCreator template class with
   M_REGISTER_ANIMATION-macro.
 */
class M_CORE_EXPORT MAnimationCreatorBase
{
public:
    /*!
       Constructor
       Registers this creator instance to MClassFactory.
     */
    MAnimationCreatorBase(const char *animationClassName);

    /*!
       Destructor
       Unregisters this creator instance from MClassFactory.
     */
    virtual ~MAnimationCreatorBase();

    /*!
       Returns a new animation instance.
       Ownership is transferred to caller.
     */
    virtual QAbstractAnimation *create() const = 0;
};

template<class ANIMATION>
class MAnimationCreator : public MAnimationCreatorBase
{
public:
    MAnimationCreator(const char *animationClassName) :
        MAnimationCreatorBase(animationClassName)
    {}
    virtual ~MAnimationCreator()
    {}

    virtual QAbstractAnimation *create() const {
        return new ANIMATION();
    }
};

#endif

