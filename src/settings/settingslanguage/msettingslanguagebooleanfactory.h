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

#ifndef MSETTINGSLANGUAGEBOOLEANFACTORY_H
#define MSETTINGSLANGUAGEBOOLEANFACTORY_H

#include <QString>

class MWidgetController;
class MSettingsLanguageWidget;
class MDataStore;
class MSettingsLanguageBoolean;

//! \internal
/*!
 * A factory for translating settings binaries to widgets.
 */
class MSettingsLanguageBooleanFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    MSettingsLanguageBooleanFactory() {}
    Q_DISABLE_COPY(MSettingsLanguageBooleanFactory)

public:
    /*!
     * \brief Creates a widget from a MSettingsLanguageBool representation.
     *
     * \param settingsBool the settings binary representation.
     * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static MWidgetController *createWidget(const MSettingsLanguageBoolean &settingsBool, MSettingsLanguageWidget &rootWidget, MDataStore *dataStore = NULL);

};
//! \internal_end

#endif // MSETTINGSLANGUAGEBOOLEANFACTORY_H
