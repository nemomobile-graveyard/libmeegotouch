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

#ifndef DUISETTINGSLANGUAGEBOOLEANFACTORY_H
#define DUISETTINGSLANGUAGEBOOLEANFACTORY_H

#include <QString>

class DuiWidgetController;
class DuiSettingsLanguageWidget;
class DuiDataStore;
class DuiSettingsLanguageBoolean;

//! \internal
/*!
 * A factory for translating settings binaries to widgets.
 */
class DuiSettingsLanguageBooleanFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    DuiSettingsLanguageBooleanFactory() {}
    Q_DISABLE_COPY(DuiSettingsLanguageBooleanFactory)

public:
    /*!
     * \brief Creates a widget from a DuiSettingsLanguageBool representation.
     *
     * \param settingsBool the settings binary representation.
     * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static DuiWidgetController *createWidget(const DuiSettingsLanguageBoolean &settingsBool, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore = NULL);

};
//! \internal_end

#endif // DUISETTINGSLANGUAGEBOOLEANFACTORY_H
