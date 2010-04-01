/***************************************************************************
**
** Copyright (C) 2010 Nokia Corporation and/or its subsidiary(-ies).
** All rights reserved.
** Contact: Nokia Corporation (directui@nokia.com)
**
** This file is part of libdui.
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

#ifndef DUIANIMATIONCREATOR_H
#define DUIANIMATIONCREATOR_H

#include "duiexport.h"

#define DUI_REGISTER_ANIMATION(ANIMATION) \
    static const DuiAnimationCreator<ANIMATION> g_AnimationCreator(#ANIMATION);

// forward declarations
class DuiAnimation;

/*!
   Interface for DuiAnimationCreators
   you can implement your own creator or use DuiAnimationCreator template class with
   DUI_REGISTER_ANIMATION-macro.
 */
class DUI_EXPORT DuiAnimationCreatorBase
{
public:
    /*!
       Constructor
       Registers this creator instance to DuiClassFactory.
     */
    DuiAnimationCreatorBase(const char *animationClassName);

    /*!
       Destructor
       Unregisters this creator instance from DuiClassFactory.
     */
    virtual ~DuiAnimationCreatorBase();

    /*!
       Returns a new animation instance.
       Ownership is transferred to caller.
     */
    virtual DuiAnimation *create() const = 0;
};

template<class ANIMATION>
class DUI_EXPORT DuiAnimationCreator : public DuiAnimationCreatorBase
{
public:
    DuiAnimationCreator(const char *animationClassName) :
        DuiAnimationCreatorBase(animationClassName)
    {}
    virtual ~DuiAnimationCreator()
    {}

    virtual DuiAnimation *create() const {
        return new ANIMATION();
    }
};

#endif

