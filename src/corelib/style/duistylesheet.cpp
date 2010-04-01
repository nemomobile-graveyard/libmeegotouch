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

#include <QList>
#include <string> // intermediate type between QString and C string
#include <DuiDebug>
#include <QColor>
#include <QPixmap>
#include <QSize>
#include <QFont>
#include <QMetaProperty>
#include <QTextCharFormat>

#include "duiapplication.h"
#include "duifeedbackplayer.h"
#include "duifeedback.h"
#include "duistylesheet.h"
#include "duistylesheet_p.h"
#include "duistylesheetselector.h"
#include "duideviceprofile.h"
#include "duiwidgetstyle.h"
#include "duiscalableimage.h"
#include "duistylesheetattribute.h"
#include "duiclassfactory.h"
#include "duiwidgetcontroller.h"

/*! \cond
 * Internal helper class to temporarily store settings data when collecting
 * the correct settings from the identifier list for the target object.
 */
class DuiOriginContainer
{
public:
    DuiOriginContainer(DuiStyleSheetAttribute *val,
                       DuiStyleSheetSelector *selector,
                       unsigned int classPriority,
                       unsigned int parentPriority,
                       const QString &file) :
        value(val),
        selector(selector),
        classPriority(classPriority),
        parentPriority(parentPriority),
        filename(file) {}

    DuiStyleSheetAttribute *value;
    DuiStyleSheetSelector *selector;
    unsigned int classPriority;
    unsigned int parentPriority;
    QString filename;
};
//! \endcond

QHash<QString, DuiStyleSheetPrivate::CacheEntry *> DuiStyleSheetPrivate::EntryCache;
QHash<QString, DuiStyle *> DuiStyleSheetPrivate::StyleCache;

#ifdef DUI_THEMESYSTEM_PROFILING_SUPPORT
int sizeOfString(const QString &string)
{
    int size = 0;

    const QChar *data = string.constData();
    while (*data++ != 0)
        size++;

    return size;
}
#endif

DuiStyleSheet::DuiStyleSheet(const DuiLogicalValues *logicalValues) : DuiStyleSheetParser(logicalValues)
{
}

DuiStyleSheet::~DuiStyleSheet()
{
}

bool DuiStyleSheetPrivate::populateStyle(const QList<const DuiStyleSheet *>& sheets,
        DuiStyle *style,
        const QString &objectName,
        const QString &mode,
        const QString &type,
        Dui::Orientation orientation,
        const DuiWidgetController *parent,
        const QList<SelectorInfo>& parentInfo)
{
    DuiStyleSheetPrivate::Key key(style->metaObject()->className(), objectName, mode, type, orientation, NULL);
    QString identifier = key.uniqueId();

    // try to get matching data (without scene graph-specific styling) from cache
    DuiStyleSheetPrivate::CacheEntry *entry = DuiStyleSheetPrivate::EntryCache.value(identifier, NULL);

    if (!entry) {
        // did not found one from cache, create one
        entry = DuiStyleSheetPrivate::buildCacheEntry(sheets, *(style->metaObject()), objectName, mode, type, orientation, parent);
        DuiStyleSheetPrivate::EntryCache.insert(identifier, entry);
    }

    // combine style without scene graph-specific attributes with the scene graph specific selectors
    if (!DuiStyleSheetPrivate::combine(style, *entry, parentInfo, orientation)) {

        // failed, report error
        qCritical("Failed to populate style for: %s#%s.%s:%s",
                  style->metaObject()->className(),
                  objectName.toStdString().c_str(),
                  (orientation == Dui::Landscape) ? "Landscape" : "Portrait",
                  mode.toStdString().c_str());

        return false;
    }

    return true;
}

DuiStyle *DuiStyleSheet::style(const QList<const DuiStyleSheet *>& sheets,
                               const QString &styleClassName,
                               const QString &objectName,
                               const QString &mode,
                               const QString &type,
                               Dui::Orientation orientation,
                               const DuiWidgetController *parent)
{
    const QMetaObject *styleMetaObject = DuiClassFactory::instance()->styleMetaObject(styleClassName.toStdString().c_str());
    if (!styleMetaObject)
        return NULL;

    // match scene graph to styling
    QList<DuiStyleSheetPrivate::SelectorInfo> results;
    DuiStyleSheetPrivate::getMatchingSelectorsWithParent(sheets, *styleMetaObject, objectName, mode, type, orientation, parent, results);

    // create unique key for the cache
    DuiStyleSheetPrivate::Key key(styleClassName, objectName, mode, type, orientation, &results);
    QString identifier = key.uniqueId();

    // try to find it from the style cache
    DuiStyle *style = DuiStyleSheetPrivate::StyleCache.value(identifier, NULL);
    if (style) {
        style->addReference();
        return style;
    }

    // cannot found this kind of style from cache, create new one
    style = DuiClassFactory::instance()->createStyle(styleClassName.toStdString().c_str());
    if (!style) {
        duiWarning("duistylesheet.cpp") << "could not create style:" << styleClassName;
        return NULL;
    }

    // populate it
    if (!DuiStyleSheetPrivate::populateStyle(sheets, style, objectName, mode, type, orientation, parent, results)) {
        delete style;
        return NULL;
    }

    //store this one to cache
    DuiStyleSheetPrivate::StyleCache.insert(identifier, style);

    return style;
}

void DuiStyleSheet::releaseStyle(const DuiStyle *style)
{
    // TODO: Optimize this later
    QHash<QString, DuiStyle *>::iterator end = DuiStyleSheetPrivate::StyleCache.end();
    for (QHash<QString, DuiStyle *>::iterator iterator = DuiStyleSheetPrivate::StyleCache.begin();
            iterator != end;
            ++iterator) {

        DuiStyle *cached = *iterator;

        // check if this was the style which needs to be released
        if (style == cached) {
            int count = cached->removeReference();

            // check if the refcount drops down to zero, if it does, delete the style
            if (count == 0) {
                // remove cache entry
                DuiStyleSheetPrivate::StyleCache.erase(iterator);
            }
            return;
        }
    }

    // unknown style object, or cache is disabled
    const_cast<DuiStyle *>(style)->removeReference();
}

void DuiStyleSheet::cleanup(bool outputAndDelete)
{
    // delete entry cache
    QHash<QString, DuiStyleSheetPrivate::CacheEntry *>::iterator end = DuiStyleSheetPrivate::EntryCache.end();
    for (QHash<QString, DuiStyleSheetPrivate::CacheEntry *>::iterator iterator = DuiStyleSheetPrivate::EntryCache.begin();
            iterator != end;
            ++iterator) {

        DuiStyleSheetPrivate::CacheEntry *entry = *iterator;
        qDeleteAll(*entry);
        delete entry;
    }
    DuiStyleSheetPrivate::EntryCache.clear();

    // print identifiers from all styles which were not released
    if (outputAndDelete) {
        QHash<QString, DuiStyle *>::iterator end = DuiStyleSheetPrivate::StyleCache.end();
        for (QHash<QString, DuiStyle *>::iterator iterator = DuiStyleSheetPrivate::StyleCache.begin();
                iterator != end;
                ++iterator) {

            DuiStyle *style = *iterator;
            duiWarning("duistylesheet.cpp") << "Style:" << iterator.key() << "not released!" << "refcount:" << style->references();
            while (style->removeReference() > 0) ;
        }
    }

    DuiStyleSheetPrivate::StyleCache.clear();
}

DuiStyleSheetPrivate::CacheEntry *DuiStyleSheetPrivate::buildCacheEntry(const QList<const DuiStyleSheet *>& sheets,
        const QMetaObject &styleMetaObject,
        const QString &objectName,
        const QString &mode,
        const QString &type,
        Dui::Orientation orientation,
        const DuiWidgetController *parent)
{
    CacheEntry *entry = new CacheEntry;

    foreach(const DuiStyleSheet * sheet, sheets) {
        foreach(DuiStyleSheetParser::StylesheetFileInfo * fi, sheet->fileInfoList()) {
            foreach(DuiStyleSheetSelector * selector, fi->selectors) {
                // duiDebug("DuiStyleSheet") << "Searching style data for: " << cname << '#' << objectName;
                // duiDebug("DuiStyleSheet") << "Comparing against" << selector->className() << '#' << selector->objectName();

                unsigned int parentPriority, classPriority;
                if (!DuiStyleSheetPrivate::match(selector, styleMetaObject, objectName, mode, type, orientation, parent, classPriority, parentPriority))
                    continue;

                // None of the early out tests failed, so we're happy with these settings, loop 'em through and copy them to list
                DuiAttributeList::const_iterator attributesEnd = selector->attributes()->constEnd();

                for (DuiAttributeList::const_iterator j = selector->attributes()->constBegin(); j != attributesEnd; ++j) {
                    QString propertyName = j.key();

                    CacheEntry::iterator iter = (*entry).find(propertyName);
                    // Check if these settings are already in the list
                    if (iter != entry->end()) {
                        // Ok so they are, we need to determine whether we have higher priority settings to replace
                        DuiOriginContainer *existing = *iter;

                        // If the previous one didn't have object name or we're higher priority, we can override
                        if (DuiStyleSheetPrivate::isHigherPriority(existing, selector, classPriority, parentPriority)) {
                            // override
                            existing->selector = selector;
                            existing->classPriority = classPriority;
                            existing->parentPriority = parentPriority;
                            existing->filename = fi->filename;
                            existing->value = j.value();
                        }
                    } else {
                        // New settings, just add them to the hash
                        (*entry)[propertyName] = new DuiOriginContainer(j.value(),
                                selector,
                                classPriority,
                                parentPriority,
                                fi->filename);
                    }
                }
            }
        }
    }

    return entry;
}

bool DuiStyleSheetPrivate::combine(DuiStyle *style, const CacheEntry &entry, const QList<SelectorInfo>& parentInfo, Dui::Orientation orientation)
{
    // we need to match data with parent information for the cache entry, this will
    // result to a combined list of attributes, which can be populated for the style

    // create copy of cache entry, we can't modify the original one for ovbious reasons
    CacheEntry data = entry;
    QList<DuiOriginContainer *> tempDuiOriginContainers;

    // match parent selectors to cached data
    foreach(const SelectorInfo & info, parentInfo) {

        if (!info.selector->attributes())
            continue;

        // check all the attributes of this selector against the cached entry
        foreach(DuiStyleSheetAttribute * attribute, *(info.selector->attributes())) {
            DuiOriginContainer *old = data.value(attribute->name, NULL);
            if (old) {
                // check which one has higher priority
                if (!isHigherPriority(old, info.selector, info.classPriority, info.parentPriority)) {
                    continue;
                }
            }

            // override
            DuiOriginContainer *tempDuiOriginCont =   new DuiOriginContainer(attribute, info.selector, info.classPriority, info.parentPriority, info.filename);
            data[attribute->name] = tempDuiOriginCont;
            tempDuiOriginContainers.append(tempDuiOriginCont);
        }
    }

    bool result = true;

    // now loop through all properties and fill them to the style object
    const int propertyCount = style->metaObject()->propertyCount();
    for (int i = DuiStyle::staticMetaObject.propertyOffset(); i != propertyCount; ++i) {

        bool propertyInitialized = false;

        // find matching attribute from hash
        CacheEntry::iterator iterator = data.find(QString(style->metaObject()->property(i).name()));
        if (iterator != data.end()) {

            // get the attribute value
            DuiOriginContainer *container = *iterator;
            DuiStyleSheetAttribute *attribute = container->value;

            // erase this iterator from the data block, it is not needed anymore
            data.erase(iterator);

            if (!attribute->writeAttribute(container->filename, style, style->metaObject()->property(i), orientation)) {
                qFatal("Failed to write attribute: %s to property %s. The stylesheet syntax might be invalid (%s:%s) in %s.",
                       attribute->name.toStdString().c_str(),
                       style->metaObject()->property(i).name(),
                       attribute->name.toStdString().c_str(),
                       attribute->value.toStdString().c_str(),
                       container->filename.toStdString().c_str());

                result = false;
            } else {
                propertyInitialized = true;
            }
        }

        // report error
        if (propertyInitialized == false) {
            duiWarning("duistylesheet.cpp") << "Property" <<
                                            style->metaObject()->property(i).name() <<
                                            "was left uninitialized in" << style->metaObject()->className();
        }
    }

    // report unused attributes
    foreach(DuiOriginContainer * container, data) {

        // build identifier for the unused selector block
        QString identifier = '(' + container->selector->className();
        if (!container->selector->classType().isEmpty())
            identifier += '[' + container->selector->classType() + ']';
        if (!container->selector->objectName().isEmpty())
            identifier += '#' + container->selector->objectName();
        if (!container->selector->orientation().isEmpty())
            identifier += '.' + container->selector->orientation();
        if (!container->selector->mode().isEmpty())
            identifier += ':' + container->selector->mode();
        identifier += ')';

        duiWarning("duistylesheet.cpp") << "unused attribute in" << container->filename << container->value->name << identifier;
    }

    // cleaning up temporary data so "data" variable  is useless after that!
    foreach(DuiOriginContainer * tempCont, tempDuiOriginContainers) {
        delete tempCont;
    }

    return result;
}

bool DuiStyleSheetPrivate::isHigherPriority(DuiOriginContainer *prev,
        DuiStyleSheetSelector *selector,
        unsigned int classPriority,
        unsigned int parentPriority)
{
    // At this stage we either have a correct object name or we don't have object name at all.
    // So, select the one which has it. If both have same name or neither one has it, go further.
    int objectName = selector->objectName().length() - prev->selector->objectName().length();
    if (objectName != 0)
        return (objectName < 0) ? false : true;

    // Only other has mode, mode is more important than orientation
    int mode = selector->mode().length() - prev->selector->mode().length();
    if (mode != 0)
        return (mode < 0) ? false : true;

    // Other one has class type and another doesn't have it
    int classType = selector->classType().length() - prev->selector->classType().length();
    if (classType != 0)
        return (classType < 0) ? false : true;


    // The closer one in the scene chain has more priority, 0xffff means no match
    unsigned int sceneOrder = EXTRACT_SCENEORDER(parentPriority);
    unsigned int inheritanceOrder = EXTRACT_INHERITANCEORDER(parentPriority);
    unsigned int previousSceneOrder = EXTRACT_SCENEORDER(prev->parentPriority);
    unsigned int previousInheritanceOrder = EXTRACT_INHERITANCEORDER(prev->parentPriority);

    // scene order
    if ((sceneOrder != 0xffff) || (previousSceneOrder != 0xffff)) {
        if (sceneOrder != previousSceneOrder) {
            return sceneOrder < previousSceneOrder;
        }
        // direct child ?
        else if (((prev->selector->flags() & DuiStyleSheetSelector::Child) != (selector->flags() & DuiStyleSheetSelector::Child)) && (inheritanceOrder == previousInheritanceOrder)) {
            return (selector->flags() & DuiStyleSheetSelector::Child) ? true : false;
        }
    }

    // inheritance order
    if ((inheritanceOrder != 0xffff) || (previousInheritanceOrder != 0xffff)) {
        if (inheritanceOrder != previousInheritanceOrder) {
            return inheritanceOrder < previousInheritanceOrder;
        }
    }

    // Only other has orientation
    int orientation = selector->orientation().length() - prev->selector->orientation().length();
    if (orientation != 0)
        return (orientation < 0) ? false : true;

    // Check the level of inheritance in class hierarchy for the existing entry for both entries
    inheritanceOrder = EXTRACT_INHERITANCEORDER(classPriority);
    previousInheritanceOrder = EXTRACT_INHERITANCEORDER(prev->classPriority);

    // Check if the new one is closer to the requested class type in class hierarchy
    if (inheritanceOrder < previousInheritanceOrder)
        return true;
    if (inheritanceOrder > previousInheritanceOrder)
        return false;

    // -> Same level of inheritance

    // These are completely equal, use the new one
    return true;
}


// calculates "order" number (how far is in superclass inheritance tree)
int DuiStyleSheetPrivate::orderNumber(const QString &n, const QMetaObject *mobj)
{
    // Selectors class name may have "." in front of it, remove it
    QStringRef realClassName;
    if (n.startsWith('.')) {
        realClassName = QStringRef(&n, 1, n.length() - 1);
    } else {
        realClassName = QStringRef(&n);
    }

    int found = -1;
    int indx = 0;

    // Calculate level of inheritance
    const QMetaObject *metaObject = mobj;
    while (metaObject) {
        if (realClassName == metaObject->className()) {
            found = indx;
            break;
        }

        metaObject = metaObject->superClass();
        indx++;
    }

    return found;
}

bool DuiStyleSheetPrivate::match(DuiStyleSheetSelector *selector,
                                 const QMetaObject &styleMetaObject,
                                 const QString &objectName,
                                 const QString &mode,
                                 const QString &type,
                                 Dui::Orientation orientation,
                                 const DuiWidgetController *parent,
                                 unsigned int &classPriority,
                                 unsigned int &parentPriority)
{
    // Initialize
    classPriority = MAKE_PRIORITY(0xffff, 0xffff);
    parentPriority = MAKE_PRIORITY(0xffff, 0xffff);

    // Check whether parent matches
    if (!selector->parentName().isEmpty()) {
        // This figures out whether the parent derives from the requested parent name
        bool found = false;
        int sceneOrder = 0;

        const QGraphicsItem *p = parent;
        while (p) {
            const QGraphicsWidget *widget = dynamic_cast<const QGraphicsWidget *>(p);
            if (widget) {

                // Check whether the parent class derives from the given one
                int inheritanceOrder = DuiStyleSheetPrivate::orderNumber(selector->parentName(), widget->metaObject());
                if (inheritanceOrder != -1) {
                    parentPriority = MAKE_PRIORITY(sceneOrder, inheritanceOrder);
                    found = true;
                    break;
                }
            }

            if (selector->flags() & DuiStyleSheetSelector::Child) {
                // Only direct parent will do.
                break;
            }
            sceneOrder++;
            p = p->parentItem();
        }

        if (!found)
            return false;
    }

    // Check whether we need to take care of the object names
    if (!selector->objectName().isEmpty()) {
        // Early out: check that object name matches, if it doesn't match we can forget these settings
        if (objectName != selector->objectName())
            return false;
    }

    // Check whether the class name matches
    if (!selector->className().isEmpty()) {
        int order = 0;
        // Check if the class name contains '.' as a first letter, if it does it means that these are not for subclasses
        if (selector->flags() & DuiStyleSheetSelector::ExactClassMatch) {

            // Early out: check that the class name is valid
            if (selector->className() != styleMetaObject.className())
                return false;
        } else {
            // Early out: Make sure that we are a class of requested type or it's subclass
            order = DuiStyleSheetPrivate::orderNumber(selector->className(), &styleMetaObject);
            if (order == -1)
                return false;
        }
        // store inheritance order for later use
        classPriority = MAKE_PRIORITY(0xffff, order);
    }

    // Check whether the orientation matches
    if (selector->orientation().length() > 0) {
        // Landscape vs. "Landscape"
        if ((orientation == Dui::Landscape) && (selector->orientation() != "Landscape"))
            return false;

        // Portrait vs. "Portrait"
        if ((orientation == Dui::Portrait) && (selector->orientation() != "Portrait"))
            return false;
    }

    // Check whether the type matches
    if (!selector->classType().isEmpty() && type != selector->classType())
        return false;

    // Check whether the mode matches
    if (!selector->mode().isEmpty() && mode != selector->mode())
        return false;

    return true;
}


void DuiStyleSheetPrivate::getMatchingSelectorsWithParent(const QList<const DuiStyleSheet *>& sheets,
        const QMetaObject &styleMetaObject,
        const QString &objectName,
        const QString &mode,
        const QString &type,
        Dui::Orientation orientation,
        const DuiWidgetController *parent,
        QList<SelectorInfo>& results)
{
    foreach(const DuiStyleSheet * sheet, sheets) {
        foreach(const DuiStyleSheetParser::StylesheetFileInfo * fi, sheet->fileInfoList()) {

            // loop trough all the selectors and find matching ones
            unsigned int parentPriority, classPriority;
            foreach(DuiStyleSheetSelector * selector, fi->parentSelectors) {
                if (match(selector, styleMetaObject, objectName, mode, type, orientation, parent, classPriority, parentPriority)) {

                    // match found, store it to results list
                    SelectorInfo info;
                    info.filename = fi->filename;
                    info.selector = selector;
                    info.classPriority = classPriority;
                    info.parentPriority = parentPriority;
                    results.append(info);
                }
            }
        }
    }
}

