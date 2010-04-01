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

#include "duilogicalvalues.h"
#include "duilogicalvalues_p.h"
#include "duistylesheetattribute.h"
#include "duidebug.h"

#include <QDir>
#include <QFile>
#include <QStringList>
#include <QTextCodec>

DuiLogicalValues::DuiLogicalValues() :
    d_ptr(new DuiLogicalValuesPrivate)
{
}

DuiLogicalValues::~DuiLogicalValues()
{
    delete d_ptr;
}

bool DuiLogicalValuesPrivate::parse(const QString &filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QString group = "General";
    data.insert("General", Values());

    while (!file.atEnd()) {
        QString line = file.readLine().trimmed();
        // skip comments
        if (line.startsWith("[")) {
            // parse group header
            int index = line.indexOf("]", 1);
            if (index == -1) {
                duiWarning("DuiLogicalValues") << "Error occurred when parsing .ini file:" << line;
                file.close();
                return false;
            }
            // this will be the currently active group
            group = line.mid(1, index - 1);
        } else {

            // key/value pair
            QString key, value;
            QString *target = &key;

            // stores the last 'good' character
            int truncation = 0;
            // go through whole line
            for (int i = 0; i < line.length(); i++) {
                QChar character = line[i];
                if (character == ';') {
                    break;
                } else if (character == '=') {
                    // remove trailing whitespaces
                    target->truncate(truncation);
                    // start to parse value
                    target = &value;
                    truncation = 0;
                } else {
                    if (target->isEmpty() && character.isSpace()) {
                        // do not add whitespaces at the beginning
                    } else {
                        (*target) += character;
                        if (!character.isSpace())
                            truncation = target->length();
                    }
                }
            }
            // remove trailing whitespaces
            target->truncate(truncation);

            // consistency check
            if (!line.startsWith(';') && line.length() > 0) {
                if (key.isEmpty() || value.isEmpty()) {
                    duiWarning("DuiLogicalValues") << "Error occurred when parsing .ini file:" << line;
                    file.close();
                    return false;
                }
                // store
                if (!data.contains(group))
                    data.insert(group, Values());

                Values &values = data[group];

                if (!values.contains(key)) {
                    values.insert(key, value);
                }
            }
        }
    }

    file.close();
    return true;
}

bool DuiLogicalValues::append(const QString &filename)
{
    Q_D(DuiLogicalValues);

    // make sure that the file exists
    if (!QFile(filename).exists())
        return false;

    // load it
    if (!d->parse(filename))
        return false;

    return true;
}

void DuiLogicalValues::load(const QStringList &themeInheritanceChain, const QString &locale)
{
    Q_D(DuiLogicalValues);

    // clear existing config
    d->data.clear();

    // load locale-specific constant definitions
    if (!locale.isEmpty()) {

        // go through whole inheritance hierarchy
        foreach(QString path, themeInheritanceChain) {

            append(path + QString("dui") + QDir::separator() + QString("locale") + QDir::separator() + locale + QDir::separator() + QString("constants.ini"));
        }
    }

    // go through whole inheritance hierarchy
    foreach(QString path, themeInheritanceChain) {

        append(path + QString("dui") + QDir::separator() + QString("constants.ini"));
    }
}

bool DuiLogicalValues::findKey(const QString &key, QString &group, QString &value) const
{
    Q_D(const DuiLogicalValues);

    // search from every group
    for (DuiLogicalValuesPrivate::Groups::const_iterator iterator = d->data.begin(); iterator != d->data.end(); iterator++) {
        // get values from this group
        const DuiLogicalValuesPrivate::Values &values = iterator.value();

        // check if this group contains the key
        if (values.contains(key)) {
            group = iterator.key();
            value = values.value(key);
            return true;
        }
    }

    return false;
}


bool DuiLogicalValues::value(const QString &group, const QString &key, QString &value) const
{
    Q_D(const DuiLogicalValues);
    if (!d->data.contains(group)) {
        duiWarning("DuiLogicalValues") << "No such group:" << group;
        return false;
    }

    const DuiLogicalValuesPrivate::Values &values = d->data[group];

    if (!values.contains(key)) {
        duiWarning("DuiLogicalValues") << "No such key:" << group << '/' << key;
        return false;
    }

    value = values.value(key);
    return true;
}


QColor DuiLogicalValues::color(const QString &group, const QString &key) const
{
    QString string;
    if (!value(group, key, string)) {
        return QColor();
    }

    bool conversionOk = false;
    QColor color = DuiStyleSheetAttribute::colorFromString(string, &conversionOk);
    if (!conversionOk) {
        duiWarning("DuiLogicalValues") << "Invalid logical color definition for" << key << ':' << string;
    }
    return color;
}

QFont DuiLogicalValues::font(const QString &group, const QString &key) const
{
    QString string;
    if (!value(group, key, string)) {
        return QFont();
    }

    bool conversionOk = false;
    QFont font = DuiStyleSheetAttribute::fontFromString(string, &conversionOk);
    if (!conversionOk) {
        duiWarning("DuiLogicalValues") << "Invalid logical font definition for" << key << ':' << string;
    }
    return font;
}
