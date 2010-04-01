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

#ifndef DUISETTINGSLANGUAGESETTINGSFACTORY_H_
#define DUISETTINGSLANGUAGESETTINGSFACTORY_H_

#include <QObject>

class QGraphicsLinearLayout;
class DuiWidgetController;
class DuiDataStore;
class DuiSettingsLanguageNode;
class DuiSettingsLanguageSettings;
class DuiSettingsLanguageWidget;

//! \internal
/*!
 * A factory for translating settings binaries to widgets.
 */
class DuiSettingsLanguageSettingsFactory
{
    /*!
     * Private constructor to prevent construction.
     */
    DuiSettingsLanguageSettingsFactory() {}
    Q_DISABLE_COPY(DuiSettingsLanguageSettingsFactory)

public:
    /*!
     * \brief Creates a widget from a DuiSettingsLanguageSettings representation.
     *
     * \param settingsItem the settings binary representation.
     * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
     * \param dataStore the data store for the settings.
     * \return a widget.
     */
    static DuiWidgetController *createWidget(const DuiSettingsLanguageSettings &settingsItem, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore = NULL);

private:

    /*!
    *  Creates child widgets to a layout from a DuiSettingsLanguageNode representation.
    *
    * \param layout the layout where widgets are added.
    * \param node the nodes binary representation.
    * \param rootWidget the root of the widget hierarchy where the new widget will be attached.
    * \param dataStore the data store for the settings.
    */
    static void createChildren(QGraphicsLinearLayout &layout, const DuiSettingsLanguageNode &node, DuiSettingsLanguageWidget &rootWidget, DuiDataStore *dataStore);
};
//! \internal_end

#endif /* DUISETTINGSLANGUAGESETTINGSFACTORY_H_ */
