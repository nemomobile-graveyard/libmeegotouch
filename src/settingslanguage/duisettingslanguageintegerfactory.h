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

#ifndef DUISETTINGSLANGUAGEINTEGERFACTORY_H_
#define DUISETTINGSLANGUAGEINTEGERFACTORY_H_

class DuiWidgetController;
class DuiSettingsLanguageInteger;
class DuiSettingsLanguageWidget;
class DuiDataStore;

/*!
 * A factory for translating settings binaries to widgets.
 */
class DuiSettingsLanguageIntegerFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    DuiSettingsLanguageIntegerFactory() {}
    Q_DISABLE_COPY(DuiSettingsLanguageIntegerFactory)

public:
    /*!
     * \brief Creates a widget from a DuiSettingsLanguageInteger representation.
     *
     * \param settingsInteger the settings binary representation.
     * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static DuiWidgetController *createWidget(const DuiSettingsLanguageInteger &settingsInteger, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore = NULL);
};

#endif /* DUISETTINGSLANGUAGEINTEGERFACTORY_H_ */
