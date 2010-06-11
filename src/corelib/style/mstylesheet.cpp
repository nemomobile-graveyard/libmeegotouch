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

#include <QList>
#include <string> // intermediate type between QString and C string
#include <MDebug>
#include <QColor>
#include <QPixmap>
#include <QSize>
#include <QFont>
#include <QMetaProperty>
#include <QTextCharFormat>

#include "mapplication.h"
#include "mfeedbackplayer.h"
#include "mfeedback.h"
#include "mstylesheet.h"
#include "mstylesheet_p.h"
#include "mstylesheetselector.h"
#include "mdeviceprofile.h"
#include "mwidgetstyle.h"
#include "mscalableimage.h"
#include "mstylesheetattribute.h"
#include "mclassfactory.h"
#include "mwidgetcontroller.h"

/*! \cond
 * Internal helper class to temporarily store settings data when collecting
 * the correct settings from the identifier list for the target object.
 */
class MOriginContainer
{
public:
    MOriginContainer(MStyleSheetAttribute *val,
                       MStyleSheetSelector *selector,
                       unsigned int classPriority,
                       unsigned int parentPriority,
                       const QString &file) :
        value(val),
        selector(selector),
        classPriority(classPriority),
        parentPriority(parentPriority),
        filename(file) {}

    MStyleSheetAttribute *value;
    MStyleSheetSelector *selector;
    unsigned int classPriority;
    unsigned int parentPriority;
    QString filename;
};
//! \endcond

QHash<QString, MStyleSheetPrivate::CacheEntry *> MStyleSheetPrivate::EntryCache;
QHash<QString, MStyle *> MStyleSheetPrivate::StyleCache;

#ifdef M_THEMESYSTEM_PROFILING_SUPPORT
int sizeOfString(const QString &string)
{
    int size = 0;

    const QChar *data = string.constData();
    while (*data++ != 0)
        size++;

    return size;
}
#endif

MStyleSheet::MStyleSheet(const MLogicalValues *logicalValues) : MStyleSheetParser(logicalValues)
{
}

MStyleSheet::~MStyleSheet()
{
}

bool MStyleSheetPrivate::populateStyle(const QList<const MStyleSheet *>& sheets,
        MStyle *style,
        const QString &objectName,
        const QString &mode,
        const QString &type,
        M::Orientation orientation,
        const MWidgetController *parent,
        const QList<SelectorInfo>& parentInfo)
{
    MStyleSheetPrivate::Key key(style->metaObject()->className(), objectName, mode, type, orientation, NULL);
    QString identifier = key.uniqueId();

    // try to get matching data (without scene graph-specific styling) from cache
    MStyleSheetPrivate::CacheEntry *entry = MStyleSheetPrivate::EntryCache.value(identifier, NULL);

    if (!entry) {
        // did not found one from cache, create one
        entry = MStyleSheetPrivate::buildCacheEntry(sheets, *(style->metaObject()), objectName, mode, type, orientation, parent);
        MStyleSheetPrivate::EntryCache.insert(identifier, entry);
    }

    // combine style without scene graph-specific attributes with the scene graph specific selectors
    if (!MStyleSheetPrivate::combine(style, *entry, parentInfo, orientation)) {

        // failed, report error
        qCritical("Failed to populate style for: %s#%s.%s:%s",
                  style->metaObject()->className(),
                  objectName.toStdString().c_str(),
                  (orientation == M::Landscape) ? "Landscape" : "Portrait",
                  mode.toStdString().c_str());

        return false;
    }

    return true;
}

MStyle *MStyleSheet::style(const QList<const MStyleSheet *>& sheets,
                               const QString &styleClassName,
                               const QString &objectName,
                               const QString &mode,
                               const QString &type,
                               M::Orientation orientation,
                               const MWidgetController *parent)
{
    return style(sheets,QList<QPair<const QMetaObject*, QList<const MStyleSheet*> > >(), styleClassName, objectName, mode, type, orientation, parent);
}

MStyle *MStyleSheet::style(const QList<const MStyleSheet *>& sheets,
                               const QList<QPair<const QMetaObject*, QList<const MStyleSheet*> > >& parentsSheets,
                               const QString &styleClassName,
                               const QString &objectName,
                               const QString &mode,
                               const QString &type,
                               M::Orientation orientation,
                               const MWidgetController *parent)
{
    const QMetaObject *styleMetaObject = MClassFactory::instance()->styleMetaObject(styleClassName.toStdString().c_str());
    if (!styleMetaObject)
        return NULL;

    // match scene graph to styling
    QList<MStyleSheetPrivate::SelectorInfo> results;
    MStyleSheetPrivate::getMatchingSelectorsWithParent(sheets, *styleMetaObject, parentsSheets, objectName, mode, type, orientation, parent, results);

    // create unique key for the cache
    MStyleSheetPrivate::Key key(styleClassName, objectName, mode, type, orientation, &results);
    QString identifier = key.uniqueId();

    // try to find it from the style cache
    MStyle *style = MStyleSheetPrivate::StyleCache.value(identifier, NULL);
    if (style) {
        style->addReference();
        return style;
    }

    // cannot found this kind of style from cache, create new one
    style = MClassFactory::instance()->createStyle(styleClassName.toStdString().c_str());
    if (!style) {
        mWarning("mstylesheet.cpp") << "could not create style:" << styleClassName;
        return NULL;
    }

    // populate it
    if (!MStyleSheetPrivate::populateStyle(sheets, style, objectName, mode, type, orientation, parent, results)) {
        delete style;
        return NULL;
    }

    //store this one to cache
    MStyleSheetPrivate::StyleCache.insert(identifier, style);

    return style;
}

void MStyleSheet::releaseStyle(const MStyle *style)
{
    // TODO: Optimize this later
    QHash<QString, MStyle *>::iterator end = MStyleSheetPrivate::StyleCache.end();
    for (QHash<QString, MStyle *>::iterator iterator = MStyleSheetPrivate::StyleCache.begin();
            iterator != end;
            ++iterator) {

        MStyle *cached = *iterator;

        // check if this was the style which needs to be released
        if (style == cached) {
            int count = cached->removeReference();

            // check if the refcount drops down to zero, if it does, delete the style
            if (count == 0) {
                // remove cache entry
                MStyleSheetPrivate::StyleCache.erase(iterator);
            }
            return;
        }
    }

    // unknown style object, or cache is disabled
    const_cast<MStyle *>(style)->removeReference();
}

void MStyleSheet::cleanup(bool outputAndDelete)
{
    // delete entry cache
    QHash<QString, MStyleSheetPrivate::CacheEntry *>::iterator end = MStyleSheetPrivate::EntryCache.end();
    for (QHash<QString, MStyleSheetPrivate::CacheEntry *>::iterator iterator = MStyleSheetPrivate::EntryCache.begin();
            iterator != end;
            ++iterator) {

        MStyleSheetPrivate::CacheEntry *entry = *iterator;
        qDeleteAll(*entry);
        delete entry;
    }
    MStyleSheetPrivate::EntryCache.clear();

    // print identifiers from all styles which were not released
    if (outputAndDelete) {
        QHash<QString, MStyle *>::iterator end = MStyleSheetPrivate::StyleCache.end();
        for (QHash<QString, MStyle *>::iterator iterator = MStyleSheetPrivate::StyleCache.begin();
                iterator != end;
                ++iterator) {

            MStyle *style = *iterator;
            mWarning("mstylesheet.cpp") << "Style:" << iterator.key() << "not released!" << "refcount:" << style->references();
            while (style->removeReference() > 0) ;
        }
    }

    MStyleSheetPrivate::StyleCache.clear();
}

MStyleSheetPrivate::CacheEntry *MStyleSheetPrivate::buildCacheEntry(const QList<const MStyleSheet *>& sheets,
        const QMetaObject &styleMetaObject,
        const QString &objectName,
        const QString &mode,
        const QString &type,
        M::Orientation orientation,
        const MWidgetController *parent)
{
    CacheEntry *entry = new CacheEntry;

    foreach(const MStyleSheet * sheet, sheets) {
        foreach(MStyleSheetParser::StylesheetFileInfo * fi, sheet->fileInfoList()) {
            foreach(MStyleSheetSelector * selector, fi->selectors) {
                // mDebug("MStyleSheet") << "Searching style data for: " << cname << '#' << objectName;
                // mDebug("MStyleSheet") << "Comparing against" << selector->className() << '#' << selector->objectName();

                unsigned int parentPriority, classPriority;
                if (!MStyleSheetPrivate::matchParents(selector, parent, parentPriority) ||
                    !MStyleSheetPrivate::match(selector, styleMetaObject, objectName, mode, type, orientation, classPriority)) {
                    continue;
                }

                // None of the early out tests failed, so we're happy with these settings, loop 'em through and copy them to list
                MAttributeList::const_iterator attributesEnd = selector->attributes()->constEnd();

                for (MAttributeList::const_iterator j = selector->attributes()->constBegin(); j != attributesEnd; ++j) {
                    QString propertyName = j.key();

                    CacheEntry::iterator iter = (*entry).find(propertyName);
                    // Check if these settings are already in the list
                    if (iter != entry->end()) {
                        // Ok so they are, we need to determine whether we have higher priority settings to replace
                        MOriginContainer *existing = *iter;

                        // If the previous one didn't have object name or we're higher priority, we can override
                        if (MStyleSheetPrivate::isHigherPriority(existing, selector, classPriority, parentPriority)) {
                            // override
                            existing->selector = selector;
                            existing->classPriority = classPriority;
                            existing->parentPriority = parentPriority;
                            existing->filename = fi->filename;
                            existing->value = j.value();
                        }
                    } else {
                        // New settings, just add them to the hash
                        (*entry)[propertyName] = new MOriginContainer(j.value(),
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

bool MStyleSheetPrivate::combine(MStyle *style, const CacheEntry &entry, const QList<SelectorInfo>& parentInfo, M::Orientation orientation)
{
    // we need to match data with parent information for the cache entry, this will
    // result to a combined list of attributes, which can be populated for the style

    // create copy of cache entry, we can't modify the original one for ovbious reasons
    CacheEntry data = entry;
    QList<MOriginContainer *> tempMOriginContainers;

    // match parent selectors to cached data
    foreach(const SelectorInfo & info, parentInfo) {

        if (!info.selector->attributes())
            continue;

        // check all the attributes of this selector against the cached entry
        foreach(MStyleSheetAttribute * attribute, *(info.selector->attributes())) {
            MOriginContainer *old = data.value(attribute->name, NULL);
            if (old) {
                // check which one has higher priority
                if (!isHigherPriority(old, info.selector, info.classPriority, info.parentPriority)) {
                    continue;
                }
            }

            // override
            MOriginContainer *tempMOriginCont =   new MOriginContainer(attribute, info.selector, info.classPriority, info.parentPriority, info.filename);
            data[attribute->name] = tempMOriginCont;
            tempMOriginContainers.append(tempMOriginCont);
        }
    }

    bool result = true;

    // now loop through all properties and fill them to the style object
    const int propertyCount = style->metaObject()->propertyCount();
    for (int i = MStyle::staticMetaObject.propertyOffset(); i != propertyCount; ++i) {

        bool propertyInitialized = false;

        // find matching attribute from hash
        CacheEntry::iterator iterator = data.find(QString(style->metaObject()->property(i).name()));
        if (iterator != data.end()) {

            // get the attribute value
            MOriginContainer *container = *iterator;
            MStyleSheetAttribute *attribute = container->value;

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
            mWarning("mstylesheet.cpp") << "Property" <<
                                            style->metaObject()->property(i).name() <<
                                            "was left uninitialized in" << style->metaObject()->className();
        }
    }

    // report unused attributes
    foreach(MOriginContainer * container, data) {

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

        mWarning("mstylesheet.cpp") << "unused attribute in" << container->filename << container->value->name << identifier;
    }

    // cleaning up temporary data so "data" variable  is useless after that!
    foreach(MOriginContainer * tempCont, tempMOriginContainers) {
        delete tempCont;
    }

    return result;
}

bool MStyleSheetPrivate::isHigherPriority(MOriginContainer *prev,
        MStyleSheetSelector *selector,
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
        else if (((prev->selector->flags() & MStyleSheetSelector::Child) != (selector->flags() & MStyleSheetSelector::Child)) && (inheritanceOrder == previousInheritanceOrder)) {
            return (selector->flags() & MStyleSheetSelector::Child) ? true : false;
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
int MStyleSheetPrivate::orderNumber(const QString &n, const QMetaObject *mobj)
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

bool MStyleSheetPrivate::matchParent(MStyleSheetSelector *selector,
                                     const QMetaObject* mobj,
                                     unsigned int sceneOrder,
                                     unsigned int &parentPriority)
{
    parentPriority = MAKE_PRIORITY(0xffff, 0xffff);
    // Check whether the parent class derives from the given one
    int inheritanceOrder = MStyleSheetPrivate::orderNumber(selector->parentName(), mobj);
    if (inheritanceOrder != -1) {
        parentPriority = MAKE_PRIORITY(sceneOrder, inheritanceOrder);
        return true;
    }
    return false;
}

bool MStyleSheetPrivate::matchParents(MStyleSheetSelector *selector,
                                        const MWidgetController *parent,
                                        unsigned int &parentPriority)
{
    parentPriority = MAKE_PRIORITY(0xffff, 0xffff);

    // Check whether parent matches
    if (!selector->parentName().isEmpty()) {
        // This figures out whether the parent derives from the requested parent name
        int sceneOrder = 0;

        const QGraphicsItem *p = parent;
        while (p) {
            const QGraphicsWidget *widget = dynamic_cast<const QGraphicsWidget *>(p);
            if (widget && matchParent(selector, widget->metaObject(), sceneOrder, parentPriority)) {
                return true;
            }

            if (selector->flags() & MStyleSheetSelector::Child) {
                // Only direct parent will do.
                return false;
            }
            sceneOrder++;
            p = p->parentItem();
        }
        return false;
    }
    return true;
}

bool MStyleSheetPrivate::match(MStyleSheetSelector *selector,
                                 const QMetaObject &styleMetaObject,
                                 const QString &objectName,
                                 const QString &mode,
                                 const QString &type,
                                 M::Orientation orientation,
                                 unsigned int &classPriority)
{
    // Initialize
    classPriority = MAKE_PRIORITY(0xffff, 0xffff);

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
        if (selector->flags() & MStyleSheetSelector::ExactClassMatch) {

            // Early out: check that the class name is valid
            if (selector->className() != styleMetaObject.className())
                return false;
        } else {
            // Early out: Make sure that we are a class of requested type or it's subclass
            order = MStyleSheetPrivate::orderNumber(selector->className(), &styleMetaObject);
            if (order == -1)
                return false;
        }
        // store inheritance order for later use
        classPriority = MAKE_PRIORITY(0xffff, order);
    }

    // Check whether the orientation matches
    if (selector->orientation().length() > 0) {
        // Landscape vs. "Landscape"
        if ((orientation == M::Landscape) && (selector->orientation() != "Landscape"))
            return false;

        // Portrait vs. "Portrait"
        if ((orientation == M::Portrait) && (selector->orientation() != "Portrait"))
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


void MStyleSheetPrivate::getMatchingSelectorsWithParent(const QList<const MStyleSheet *>& sheets,
        const QMetaObject &styleMetaObject,
        const QList<QPair<const QMetaObject*, QList<const MStyleSheet*> > > parentsSheets,
        const QString &objectName,
        const QString &mode,
        const QString &type,
        M::Orientation orientation,
        const MWidgetController *parent,
        QList<SelectorInfo>& results)
{
    typedef QPair<const QMetaObject*, QList<const MStyleSheet*> > MetaObjectStyleSheetList;
    unsigned int sceneOrder = 0;
    foreach(const MetaObjectStyleSheetList& parentSheets, parentsSheets) {
        const QMetaObject* mobj = parentSheets.first;
        foreach(const MStyleSheet* sheet, parentSheets.second) {
            foreach(const MStyleSheetParser::StylesheetFileInfo* fi, sheet->fileInfoList()) {
                unsigned int parentPriority, classPriority;
                foreach(MStyleSheetSelector* selector, fi->parentSelectors) {
                    if(matchParent(selector, mobj, sceneOrder, parentPriority) &&
                       match(selector, styleMetaObject, objectName, mode, type, orientation, classPriority)) {

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
        ++sceneOrder;
    }

    foreach(const MStyleSheet * sheet, sheets) {
        foreach(const MStyleSheetParser::StylesheetFileInfo * fi, sheet->fileInfoList()) {

            // loop trough all the selectors and find matching ones
            unsigned int parentPriority, classPriority;
            foreach(MStyleSheetSelector * selector, fi->parentSelectors) {
                if (matchParents(selector, parent, parentPriority) &&
                    match(selector, styleMetaObject, objectName, mode, type, orientation, classPriority)) {

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

