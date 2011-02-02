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

#ifndef MPOPUPLISTSTYLE_H
#define MPOPUPLISTSTYLE_H

#include <mdialogstyle.h>

/*!
    \class MPopupListStyle
    \brief Style class for standard MPopupList.

    MPopupListStyle is derived from MDialogStyle.

    \ingroup styles
    \sa MPopupListStyleContainer MDialogStyle \ref styling MPopupList MPopupListView
*/

class M_VIEWS_EXPORT MPopupListStyle : public MDialogStyle
{
    Q_OBJECT
    M_STYLE_INTERNAL(MPopupListStyle)

    M_STYLE_ATTRIBUTE(QString, itemStyleName, ItemStyleName)
    M_STYLE_ATTRIBUTE(QString, contentsViewportStyleName, ContentsViewportStyleName)
    M_STYLE_ATTRIBUTE(bool, itemWordWrap, ItemWordWrap)
};

/*!
    \class MPopupListStyleContainer
    \brief Style mode container class for MPopupListStyle.

    \ingroup styles
    \sa MPopupListStyle
*/
class M_VIEWS_EXPORT MPopupListStyleContainer : public MDialogStyleContainer
{
    M_STYLE_CONTAINER_INTERNAL(MPopupListStyle)
};

#endif


