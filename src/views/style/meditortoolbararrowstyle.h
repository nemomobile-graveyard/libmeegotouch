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
#ifndef MEDITORTOOLBARARROWSTYLE_H
#define MEDITORTOOLBARARROWSTYLE_H

#include <MWidgetStyle>

/*!
    \brief This defines style for the small arrow in MEditorToolbar.
*/
class M_VIEWS_EXPORT MEditorToolbarArrowStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MEditorToolbarArrowStyle)
};

class M_VIEWS_EXPORT MEditorToolbarArrowStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MEditorToolbarArrowStyle)

    //! Mode for arrow pointing up
    M_STYLE_MODE(Up)

    //! Mode for arrow pointing down
    M_STYLE_MODE(Down)
};

#endif // MEDITORTOOLBARARROWSTYLE_H
