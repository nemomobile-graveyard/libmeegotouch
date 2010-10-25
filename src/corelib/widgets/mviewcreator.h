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

#ifndef MVIEWCREATOR_H
#define MVIEWCREATOR_H

#include "mexport.h"
#include <QMetaProperty>

#define M_REGISTER_VIEW_NEW(VIEW, CONTROLLER) \
    static const MViewCreator<VIEW,CONTROLLER> g_ViewCreator(#VIEW);


#define M_REGISTER_VIEW(VIEW, CONTROLLER) \
    static const MViewCreator<VIEW,CONTROLLER> g_ViewCreator(#VIEW);

// forward declarations
class MWidgetView;
class MWidgetController;

// base class for MViewCreators
// you can implement your own creator or use MViewCreator template class with
// M_REGISTER_VIEW-macro.
class M_CORE_EXPORT MViewCreatorBase
{
public:
    MViewCreatorBase(const char *viewClassName);
    virtual ~MViewCreatorBase();
    /*!
       Returns new style instance.
       Ownership is transferred to caller.
     */
    virtual MWidgetView *create(const MWidgetController *controller) const = 0;

    /*!
       Returns the type of the style the view uses.
     */
    virtual const char *styleType() const = 0;
};

template<class VIEW, class CONTROLLER>
class MViewCreator : public MViewCreatorBase
{
public:
    MViewCreator(const char *viewClassName) :
        MViewCreatorBase(viewClassName)
    {}
    virtual ~MViewCreator()
    {}

    virtual MWidgetView *create(const MWidgetController *controller) const {
        return new VIEW((CONTROLLER *)controller);
        //return new VIEW(dynamic_cast<CONTROLLER*>(controller));
    }

    virtual const char *styleType() const {
        return VIEW::staticStyleType();
    }
};

#endif

