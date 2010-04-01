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

#ifndef DUISETTINGSLANGUAGEFACTORY_H_
#define DUISETTINGSLANGUAGEFACTORY_H_

#include <DuiExport>
#include <QString>

class DuiWidget;
class DuiSettingsLanguageWidget;
class DuiSettingsLanguageBinary;
class DuiDataStore;

/*!
 * A factory for translating settings binaries to widgets.
 */
class DUI_EXPORT DuiSettingsLanguageWidgetFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    DuiSettingsLanguageWidgetFactory() {}
    Q_DISABLE_COPY(DuiSettingsLanguageWidgetFactory)

public:
    /*!
     * \brief Creates a widget from a DuiSettingsLanguageBinary representation.
     *
     * \param settingsBinary the settings binary representation.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static DuiSettingsLanguageWidget *createWidget(const DuiSettingsLanguageBinary &settingsBinary, DuiDataStore *dataStore = NULL);
};

#endif /* DUISETTINGSLANGUAGEFACTORY_H_ */
