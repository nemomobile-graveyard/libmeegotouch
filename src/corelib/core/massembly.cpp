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

#include <QDir>
#include <QSettings>

#include "mgconfitem.h"
#include "massembly.h"
#include "massembly_p.h"
#include "mdebug.h"
#include "mwidgetcontroller.h"

bool MAssemblyPrivate::loadViewConfiguration(const QStringList &themeInheritance)
{
    viewConfiguration.clear();
    // load all view configuration files in inheritance order
    foreach(const QString & themePath, themeInheritance) {
        // resolve filename for configuration file
        QString filename = themePath + "meegotouch" + QDir::separator() + name + QDir::separator() + name + ".conf";

        // check that we have such file
        if (!QFile::exists(filename))
            continue;

        // load it
        QSettings config(filename, QSettings::IniFormat);
        if (config.status() != QSettings::NoError) {
            mWarning("MAssembly") << "Invalid view configuration file:" << filename;
            return false;
        }

        // copy all the loaded keys to our configuration table
        const QStringList keys(config.allKeys());
        foreach(const QString & key, keys) {
            QVariant value = config.value(key);
            if (!viewConfiguration.contains(key))
                viewConfiguration.insert(key, value.toString());
        }
    }

    return true;
}

void MAssemblyPrivate::loadStylesheet(const QString &filename, const MLogicalValues &logicalValues)
{
    mDebug("MAssemblyPrivate") << "load stylesheet from" << qPrintable(filename);
    // load stylesheet to new sheet
    MStyleSheet *newStylesheet = new MStyleSheet(&logicalValues);
    bool result = newStylesheet->load(filename);
    if (result) {
        // loaded succesfully
        if (stylesheet) {
            // existing stylesheet data, append
            *stylesheet += *newStylesheet;
            delete newStylesheet;
            newStylesheet = NULL;
        } else {
            // no existing data, this is the first sheet which was loaded
            stylesheet = newStylesheet;
        }
    } else {
        mWarning("MAssembly") << "Failed to load stylesheet:" << filename;
        delete newStylesheet;
        exit(2);
    }
}

void MAssemblyPrivate::loadStylesheets(const QStringList &themeInheritance, const MLogicalValues &logicalValues)
{
    delete stylesheet;
    stylesheet = NULL;

    // load in reverse order, base first
    for (int j = themeInheritance.count() - 1; j >= 0; --j) {

        QString path = themeInheritance.at(j) + "meegotouch" + QDir::separator() + name + QDir::separator();
        QString filename = path + "style" + QDir::separator() + name + ".css";

        if (QFile::exists(filename)) {
            loadStylesheet(filename, logicalValues);
        } else {
            if (j == themeInheritance.count() - 1) {
                // warnings only if the stylesheet is missing from the base theme
                mWarning("MAssembly") << "Stylesheet missing" << filename;
            }
        }
    }
}

MAssembly::MAssembly(const QString &name) :
    d_ptr(new MAssemblyPrivate)
{
    d_ptr->name = name;
    d_ptr->stylesheet = NULL;
}

MAssembly::~MAssembly()
{
    delete d_ptr->stylesheet;
    delete d_ptr;
}

QString MAssembly::name() const
{
    return d_ptr->name;
}

QString MAssembly::viewType(const MWidgetController *widget, bool &exactMatch) const
{
    QString viewClassName;

    exactMatch = false;
    // use whole inheritance chain to figure out best-matching view for this controller
    for (const QMetaObject *metaObject = widget->metaObject(); metaObject->superClass(); metaObject = metaObject->superClass()) {

        // Figure out the class name of the controller
        QString controllerClassName = metaObject->className();

        QString view = d_ptr->viewConfiguration.value(controllerClassName + '/' + widget->viewType(), QString());
        if (view.isEmpty()) {
            // try to get default if nothing has been found so far
            if (viewClassName.isEmpty())
                viewClassName = d_ptr->viewConfiguration.value(controllerClassName + "/default", QString());
        } else {
            exactMatch = true;
            viewClassName = view;
            break;
        }
    }

    return viewClassName;
}

MStyleSheet *MAssembly::stylesheet() const
{
    return d_ptr->stylesheet;
}

void MAssembly::themeChanged(const QStringList &themeInheritance, const MLogicalValues &logicalValues)
{
    // (re)load view configuration
    d_ptr->loadViewConfiguration(themeInheritance);

    // (re)load stylesheet
    d_ptr->loadStylesheets(themeInheritance, logicalValues);
}


