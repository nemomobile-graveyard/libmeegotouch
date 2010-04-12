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

#ifndef MSETTINGSLANGUAGESETTINGSFACTORYSTYLE_H_
#define MSETTINGSLANGUAGESETTINGSFACTORYSTYLE_H_

#include <MWidgetStyle>

/*!
 * MSettingsLanguageSettingsFactoryStyle is the style class for MSettingsLanguageSettingFactory.
 */
class M_EXPORT MSettingsLanguageSettingsFactoryStyle : public MWidgetStyle
{
    Q_OBJECT
    M_STYLE(MSettingsLanguageSettingsFactoryStyle)

    //! The image ID of the content indicator to be shown next to clickable items
    M_STYLE_ATTRIBUTE(QString, contentIndicator, ContentIndicator)

    //! The size of the content indicator
    M_STYLE_ATTRIBUTE(QSize, contentIndicatorSize, ContentIndicatorSize)

    // TODO: remove this when there's support from MContainer for the styling
    //! The left margin for the content indicator when it is inside a MContainer
    M_STYLE_ATTRIBUTE(qreal, contentIndicatorContainerMargin, ContentIndicatorContainerMargin)

    //! The top margin of the content indicator when it is not inside a MContainer
    M_STYLE_ATTRIBUTE(qreal, contentIndicatorTopMargin, ContentIndicatorTopMargin)

    //! The left margin of the content indicator when it is not inside a MContainer
    M_STYLE_ATTRIBUTE(qreal, contentIndicatorLeftMargin, ContentIndicatorLeftMargin)

    //! The right margin of the content indicator when it is not inside a MContainer
    M_STYLE_ATTRIBUTE(qreal, contentIndicatorRightMargin, ContentIndicatorRightMargin)

    //! The bottom margin of the content indicator when it is not inside a MContainer
    M_STYLE_ATTRIBUTE(qreal, contentIndicatorBottomMargin, ContentIndicatorBottomMargin)
};

/*!
 * MSettingsLanguageSettingsFactoryStyleContainer is the style container class for MSettingsLanguageSettingFactory.
 */
class M_EXPORT MSettingsLanguageSettingsFactoryStyleContainer : public MWidgetStyleContainer
{
    M_STYLE_CONTAINER(MSettingsLanguageSettingsFactoryStyle)
};

#endif /* MSETTINGSLANGUAGESETTINGSFACTORYSTYLE_H_ */
