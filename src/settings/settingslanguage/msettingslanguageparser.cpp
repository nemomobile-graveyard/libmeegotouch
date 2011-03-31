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

#include "msettingslanguageparser.h"
#include "msettingslanguageparser_p.h"
#include "msettingslanguagebinary.h"
#include "msettingslanguagenode.h"
#include "msettingslanguagetext.h"
#include "msettingslanguageselection.h"
#include "msettingslanguagesettings.h"
#include "msettingslanguageboolean.h"
#include "msettingslanguageinteger.h"
#include "msettingslanguagegroup.h"

#include <QFile>
#include <MDebug>

#define PARSER_WARNING mWarning("MSettingsLanguageParser")

MSettingsLanguageParserPrivate::MSettingsLanguageParserPrivate() :
    document(NULL), q_ptr(0)
{
}

MSettingsLanguageParserPrivate::~MSettingsLanguageParserPrivate()
{
    delete document;
}

MSettingsLanguageParser::MSettingsLanguageParser() :
    d_ptr(new MSettingsLanguageParserPrivate)
{
    Q_D(MSettingsLanguageParser);

    d->q_ptr = this;
}

MSettingsLanguageParser::~MSettingsLanguageParser()
{
    delete d_ptr;
}

bool MSettingsLanguageParser::readFrom(QIODevice &device)
{
    Q_D(MSettingsLanguageParser);
    delete d->document;
    d->document = new QDomDocument();
    return d->document->setContent(&device, false);
}

MSettingsLanguageBinary *MSettingsLanguageParser::createSettingsBinary()
{
    Q_D(MSettingsLanguageParser);

    if (d->document == NULL) {
        return NULL;
    }

    MSettingsLanguageBinary *sb = new MSettingsLanguageBinary;
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

bool MSettingsLanguageParserPrivate::parse(const QDomElement &item, MSettingsLanguageNode &settingsNode)
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

bool MSettingsLanguageParserPrivate::parseSettings(const QDomElement &item, MSettingsLanguageNode &settingsNode)
{
    // Possible children elements of a 'settings' element
    const static QStringList itemChildElements = (QStringList() << "text" << "boolean" << "integer" << "selection" << "group");

    MSettingsLanguageSettings *si = new MSettingsLanguageSettings;
    settingsNode.addChild(si);

    // Parse child elements
    return parseChildElements(item, *si, itemChildElements);
}

bool MSettingsLanguageParserPrivate::parseGroup(const QDomElement &item, MSettingsLanguageNode &settingsNode)
{
    // Possible children elements of a 'settings' element
    const static QStringList itemChildElements = (QStringList() << "text" << "boolean" << "integer" << "selection");
    const static QStringList requiredAttributes = (QStringList() << "title");

    MSettingsLanguageNode *group = new MSettingsLanguageGroup(item.attribute("title"));
    settingsNode.addChild(group);

    // Parse child elements
    return parseChildElements(item, *group, itemChildElements);
}

bool MSettingsLanguageParserPrivate::parseSelection(const QDomElement &item, MSettingsLanguageNode &settingsNode)
{
    // Possible children elements of a 'selection' element
    const static QStringList selectionChildElements = (QStringList() << "option");
    const static QStringList requiredAttributes = (QStringList() << "key" << "title");

    if (checkAttributes(item, requiredAttributes)) {
        MSettingsLanguageSelection *se = new MSettingsLanguageSelection(item.attribute("key"), item.attribute("title"));
        settingsNode.addChild(se);

        // Parse child elements
        return parseChildElements(item, *se, selectionChildElements);
    } else {
        return false;
    }
}

bool MSettingsLanguageParserPrivate::parseInteger(const QDomElement &item, MSettingsLanguageNode &settingsNode)
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

        MSettingsLanguageInteger *si = new MSettingsLanguageInteger(item.attribute("key"), item.attribute("title"));
        if (minOk)
            si->setMinValue(min);
        if (maxOk)
            si->setMaxValue(max);

        settingsNode.addChild(si);
        result = true;
    }

    return result;
}

bool MSettingsLanguageParserPrivate::parseOption(const QDomElement &item, MSettingsLanguageNode &settingsNode)
{
    bool result = false;
    const static QStringList requiredAttributes = (QStringList() << "title");

    if (checkAttributes(item, requiredAttributes)) {
        if (dynamic_cast<const MSettingsLanguageSelection *>(&settingsNode)) {
            bool ok;
            int value = item.text().toInt(&ok);
            if (ok) {
                static_cast<MSettingsLanguageSelection &>(settingsNode).addOption(item.attribute("title"), value);
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

bool MSettingsLanguageParserPrivate::parseText(const QDomElement &item, MSettingsLanguageNode &settingsNode)
{
    const static QStringList requiredAttributes = (QStringList() << "key" << "title");

    bool result = true;
    if (checkAttributes(item, requiredAttributes)) {
        MSettingsLanguageText *si = new MSettingsLanguageText(item.attribute("key"), item.attribute("title"));
        settingsNode.addChild(si);
    } else {
        result = false;
    }

    return result;
}

bool MSettingsLanguageParserPrivate::parseBoolean(const QDomElement &item, MSettingsLanguageNode &settingsNode)
{
    const static QStringList requiredAttributes = (QStringList() << "key" << "title");

    bool result = true;
    if (checkAttributes(item, requiredAttributes)) {
        MSettingsLanguageBoolean *sb = new MSettingsLanguageBoolean(item.attribute("key"), item.attribute("title"));
        settingsNode.addChild(sb);
    } else {
        result = false;
    }

    return result;
}

bool MSettingsLanguageParserPrivate::checkAttributes(const QDomElement &item, const QStringList &requiredAttributes) const
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

bool MSettingsLanguageParserPrivate::parseChildElements(const QDomElement &item, MSettingsLanguageNode &settingsNode, const QStringList &acceptedChildren)
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
