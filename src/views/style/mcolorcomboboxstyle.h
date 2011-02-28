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

#ifndef MCOLORCOMBOBOXSTYLE_H
#define MCOLORCOMBOBOXSTYLE_H

#include <QString>
#include <MWidgetStyle>

class M_VIEWS_EXPORT MColorComboBoxStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MColorComboBoxStyle)
    M_STYLE_ATTRIBUTE(QString, buttonStyleName, ButtonStyleName)
    M_STYLE_ATTRIBUTE(QString, titleStyleName, TitleStyleName)
    M_STYLE_ATTRIBUTE(QString, colorStyleName, ColorStyleName)
};

class M_EXPORT MColorComboBoxStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MColorComboBoxStyle)
};

#endif
