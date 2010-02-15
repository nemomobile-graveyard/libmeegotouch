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

#ifndef DUIVIEWCREATOR_H
#define DUIVIEWCREATOR_H

#include "duiexport.h"
#include <QMetaProperty>

#define DUI_REGISTER_VIEW_NEW(VIEW, CONTROLLER) \
    static const DuiViewCreator<VIEW,CONTROLLER> g_ViewCreator(#VIEW);


#define DUI_REGISTER_VIEW(VIEW, CONTROLLER) \
    static const DuiViewCreator<VIEW,CONTROLLER> g_ViewCreator(#VIEW);

// forward declarations
class DuiWidgetView;
class DuiWidgetController;

// base class for DuiViewCreators
// you can implement your own creator or use DuiViewCreator template class with
// DUI_REGISTER_VIEW-macro.
class DUI_EXPORT DuiViewCreatorBase
{
public:
    DuiViewCreatorBase(const char *viewClassName);
    virtual ~DuiViewCreatorBase();
    /*!
       Returns new style instance.
       Ownership is transferred to caller.
     */
    virtual DuiWidgetView *create(const DuiWidgetController *controller) const = 0;

    /*!
       Returns the type of the style the view uses.
     */
    virtual const char *styleType() const = 0;
};

template<class VIEW, class CONTROLLER>
class DUI_EXPORT DuiViewCreator : public DuiViewCreatorBase
{
public:
    DuiViewCreator(const char *viewClassName) :
        DuiViewCreatorBase(viewClassName)
    {}
    virtual ~DuiViewCreator()
    {}

    virtual DuiWidgetView *create(const DuiWidgetController *controller) const {
        return new VIEW((CONTROLLER *)controller);
        //return new VIEW(dynamic_cast<CONTROLLER*>(controller));
    }

    virtual const char *styleType() const {
        return VIEW::staticStyleType();
    }
};

#endif

