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

#include "duisettingslanguageparser.h"
#include "duisettingslanguageparser_p.h"
#include "duisettingslanguagebinary.h"
#include "duisettingslanguagenode.h"
#include "duisettingslanguagetext.h"
#include "duisettingslanguageselection.h"
#include "duisettingslanguagesettings.h"
#include "duisettingslanguageboolean.h"
#include "duisettingslanguageinteger.h"
#include "duisettingslanguagegroup.h"

#include <QFile>
#include <DuiDebug>

#define PARSER_WARNING duiWarning("DuiSettingsLanguageParser")

DuiSettingsLanguageParserPrivate::DuiSettingsLanguageParserPrivate() :
    document(NULL), q_ptr(0)
{
}

DuiSettingsLanguageParserPrivate::~DuiSettingsLanguageParserPrivate()
{
    delete document;
}

DuiSettingsLanguageParser::DuiSettingsLanguageParser() :
    d_ptr(new DuiSettingsLanguageParserPrivate)
{
    Q_D(DuiSettingsLanguageParser);

    d->q_ptr = this;
}

DuiSettingsLanguageParser::~DuiSettingsLanguageParser()
{
    delete d_ptr;
}

bool DuiSettingsLanguageParser::readFrom(QIODevice &device)
{
    Q_D(DuiSettingsLanguageParser);
    delete d->document;
    d->document = new QDomDocument();
    return d->document->setContent(&device, false);
}

DuiSettingsLanguageBinary *DuiSettingsLanguageParser::createSettingsBinary()
{
    Q_D(DuiSettingsLanguageParser);

    if (d->document == NULL) {
        return NULL;
    }

    DuiSettingsLanguageBinary *sb = new DuiSettingsLanguageBinary;
    QDomElement root = d->document->documentElement();
    if (root.nodeName() == "settings") {
        if (d->parse(root, *sb)) {
            return sb;
        }
    }

    // Return NULL on error
    delete sb;
    return NULL;
}

bool DuiSettingsLanguageParserPrivate::parse(const QDomElement &item, DuiSettingsLanguageNode &settingsNode)
{
    QString elementName = item.nodeName();

    if (elementName == "settings") {
        return parseSettings(item, settingsNode);
    } else if (elementName == "selection") {
        return  parseSelection(item, settingsNode);
    } else if (elementName == "option") {
        return  parseOption(item, settingsNode);
    } else if (elementName == "text") {
        return  parseText(item, settingsNode);
    } else if (elementName == "boolean") {
        return parseBoolean(item, settingsNode);
    } else if (elementName == "integer") {
        return parseInteger(item, settingsNode);
    } else if (elementName == "group") {
        return parseGroup(item, settingsNode);
    }

    return false;
}

bool DuiSettingsLanguageParserPrivate::parseSettings(const QDomElement &item, DuiSettingsLanguageNode &settingsNode)
{
    // Possible children elements of a 'settings' element
    const static QStringList itemChildElements = (QStringList() << "text" << "boolean" << "integer" << "selection" << "group");

    DuiSettingsLanguageSettings *si = new DuiSettingsLanguageSettings;
    settingsNode.addChild(si);

    // Parse child elements
    return parseChildElements(item, *si, itemChildElements);
}

bool DuiSettingsLanguageParserPrivate::parseGroup(const QDomElement &item, DuiSettingsLanguageNode &settingsNode)
{
    // Possible children elements of a 'settings' element
    const static QStringList itemChildElements = (QStringList() << "text" << "boolean"
            << "integer" << "selection");

    DuiSettingsLanguageNode *group = new DuiSettingsLanguageGroup();
    settingsNode.addChild(group);

    // Parse child elements
    return parseChildElements(item, *group, itemChildElements);
}

bool DuiSettingsLanguageParserPrivate::parseSelection(const QDomElement &item, DuiSettingsLanguageNode &settingsNode)
{
    // Possible children elements of a 'selection' element
    const static QStringList selectionChildElements = (QStringList() << "option");
    const static QStringList requiredAttributes = (QStringList() << "key");

    if (checkAttributes(item, requiredAttributes)) {
        DuiSettingsLanguageSelection *se = new DuiSettingsLanguageSelection(item.attribute("key"));
        settingsNode.addChild(se);

        // Parse child elements
        return parseChildElements(item, *se, selectionChildElements);
    } else {
        return false;
    }
}

bool DuiSettingsLanguageParserPrivate::parseInteger(const QDomElement &item, DuiSettingsLanguageNode &settingsNode)
{
    bool result = false;

    const static QStringList requiredAttributes = (QStringList() << "key" << "title");
    if (checkAttributes(item, requiredAttributes)) {
        bool minOk = true;
        bool maxOk = true;
        int min = item.attribute("min").toInt(&minOk);
        int max = item.attribute("max").toInt(&maxOk);
        if (minOk && maxOk && min > max) {
            minOk = false;
            maxOk = false;
        }

        DuiSettingsLanguageInteger *si = new DuiSettingsLanguageInteger(item.attribute("key"), item.attribute("title"));
        if (minOk)
            si->setMinValue(min);
        if (maxOk)
            si->setMaxValue(max);

        settingsNode.addChild(si);
        result = true;
    }

    return result;
}

bool DuiSettingsLanguageParserPrivate::parseOption(const QDomElement &item, DuiSettingsLanguageNode &settingsNode)
{
    bool result = false;
    const static QStringList requiredAttributes = (QStringList() << "title");

    if (checkAttributes(item, requiredAttributes)) {
        if (dynamic_cast<const DuiSettingsLanguageSelection *>(&settingsNode)) {
            bool ok;
            int value = item.text().toInt(&ok);
            if (ok) {
                static_cast<DuiSettingsLanguageSelection &>(settingsNode).addOption(item.attribute("title"), value);
                result = true;
            } else {
                PARSER_WARNING << "Contents of an \"option\" element can't be converted to an integer";
            }
        } else {
            PARSER_WARNING << "An \"option\" element must be child element of \"selection\"";
        }
    }

    return result;
}

bool DuiSettingsLanguageParserPrivate::parseText(const QDomElement &item, DuiSettingsLanguageNode &settingsNode)
{
    const static QStringList requiredAttributes = (QStringList() << "key" << "title");

    bool result = true;
    if (checkAttributes(item, requiredAttributes)) {
        DuiSettingsLanguageText *si = new DuiSettingsLanguageText(item.attribute("key"), item.attribute("title"));
        settingsNode.addChild(si);
    } else {
        result = false;
    }

    return result;
}

bool DuiSettingsLanguageParserPrivate::parseBoolean(const QDomElement &item, DuiSettingsLanguageNode &settingsNode)
{
    const static QStringList requiredAttributes = (QStringList() << "key" << "title");

    bool result = true;
    if (checkAttributes(item, requiredAttributes)) {
        DuiSettingsLanguageBoolean *sb = new DuiSettingsLanguageBoolean(item.attribute("key"), item.attribute("title"));
        settingsNode.addChild(sb);
    } else {
        result = false;
    }

    return result;
}

bool DuiSettingsLanguageParserPrivate::checkAttributes(const QDomElement &item, const QStringList &requiredAttributes) const
{
    bool result = true;
    // Check that every required attribute is specified in the DOM element
    foreach(const QString & attr, requiredAttributes) {
        if (!item.hasAttribute(attr)) {
            PARSER_WARNING << item.nodeName() << "element is missing a required attribute" << attr;
            result = false;
        }
    }

    return result;
}

bool DuiSettingsLanguageParserPrivate::parseChildElements(const QDomElement &item, DuiSettingsLanguageNode &settingsNode, const QStringList &acceptedChildren)
{
    for (QDomElement child = item.firstChildElement(); !child.isNull(); child = child.nextSiblingElement()) {
        if (acceptedChildren.contains(child.nodeName())) {
            if (!parse(child, settingsNode)) {
                // If child parsing fails, stop and return false
                return false;
            }
        } else {
            PARSER_WARNING << "Invalid child element" << child.nodeName() << "for an" << item.nodeName() << "element";
            return false;
        }
    }

    return true;
}
