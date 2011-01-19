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
#include "mtheme_p.h"
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
                       const QByteArray &file,
                       const MStyleSheet *stylesheet) :
        value(val),
        selector(selector),
        classPriority(classPriority),
        parentPriority(parentPriority),
        filename(file),
        stylesheet(stylesheet) {}

    MStyleSheetAttribute *value;
    MStyleSheetSelector *selector;
    unsigned int classPriority;
    unsigned int parentPriority;
    QByteArray filename;
    const MStyleSheet *stylesheet;
};
//! \endcond

QHash<QByteArray, MStyleSheetPrivate::CacheEntry *> MStyleSheetPrivate::EntryCache;
QHash<QByteArray, MStyle *> MStyleSheetPrivate::StyleCache;

#ifdef M_THEMESYSTEM_PROFILING_SUPPORT
int sizeOfString(const QByteArray &string)
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

MStyle *MStyleSheetPrivate::buildStyle(const StyleSpec &spec,
                                       const QList<const MStyleSheet *>& sheets,
                                       const QVector<ParentData> &parentsData,
                                       const QByteArray &parentStyleName)
{
    MStyle *style = MClassFactory::instance()->createStyle(spec.className);
    if (!style) {
        mWarning("mstylesheet.cpp") << "could not create style:" << spec.className;
        return NULL;
    }

    // We keep a second cache, the EntryCache, that stores raw data about styling, and which
    // doesn't take the scene information in account.
    QByteArray identifier = spec.entryCacheKey();
    MStyleSheetPrivate::CacheEntry *entry = MStyleSheetPrivate::EntryCache.value(identifier, NULL);

    if (!entry) {
        entry = MStyleSheetPrivate::buildCacheEntry(sheets, spec, parentsData, parentStyleName);
        // Here we maintain only the EntryCache, not the StyleCache.
        MStyleSheetPrivate::EntryCache.insert(identifier, entry);
    }

    // Fill the MStyle object by combining the information from the EntryCache with the
    // scene-dependent selectors in spec.parentInfo. After that the 'style' object will be
    // ready to be used.
    if (!MStyleSheetPrivate::combine(style, *entry, spec)) {
        qCritical("Failed to populate style for: %s#%s.%s:%s",
                  style->metaObject()->className(),
                  spec.objectName.constData(),
                  (spec.orientation == M::Landscape) ? "Landscape" : "Portrait",
                  spec.mode.constData());

        delete style;
        exit(2);
    }

    return style;
}

QVector<MStyleSheetPrivate::ParentData> MStyleSheetPrivate::extractParentsData(const MWidgetController *parent)
{
    int size = 0;
    const QGraphicsItem *p = parent;
    while (p) {
        if (p->isWidget())
            size++;
        p = p->parentItem();
    }

    QVector<ParentData> parentsData(size);
    p = parent;

    int i = 0;
    while (p) {
        if (p->isWidget()) {
            ParentData &pd = parentsData[i];

            const QGraphicsWidget *widget = static_cast<const QGraphicsWidget *>(p);
            const QMetaObject *mobj = widget->metaObject();

            while (mobj->className() != QObject::staticMetaObject.className()) { // ###
                pd.hierarchy.append(mobj->className()); // ###
                mobj = mobj->superClass();
            }

            i++;
        }
        p = p->parentItem();
    }

    return parentsData;
}

MStyle *MStyleSheet::style(const QList<const MStyleSheet *>& sheets,
                           const QByteArray &styleClassName,
                           const QByteArray &objectName,
                           const QByteArray &mode,
                           const QByteArray &type,
                           M::Orientation orientation,
                           const MWidgetController *parent)
{
    QVector<MStyleSheetPrivate::ParentData> parentsData = MStyleSheetPrivate::extractParentsData(parent);
    QList< QList<const MStyleSheet *> > parentsSheets;
    QList<QByteArray> styleMetaObjectHierarchy;

    const QMetaObject *mobj = MClassFactory::instance()->styleMetaObject(styleClassName);
    do {
        styleMetaObjectHierarchy.append(mobj->className());
        mobj = mobj->superClass();
    } while (mobj->className() != QObject::staticMetaObject.className());

    QByteArray parentStyleName;
    if (parent) {
        if (!parent->styleName().isNull())
            parentStyleName = parent->styleName().toAscii();
        else
            parentStyleName = parent->objectName().toAscii();
    }

    return MStyleSheetPrivate::style(sheets, parentsData, parentStyleName, styleMetaObjectHierarchy, styleClassName, objectName, mode, type, orientation);
}

MStyle *MStyleSheet::style(const QList<const MStyleSheet *> &,
                           const QList<QPair<const QMetaObject*, QList<const MStyleSheet*> > > &,
                           const QByteArray &,
                           const QByteArray &,
                           const QByteArray &,
                           const QByteArray &,
                           M::Orientation,
                           const MWidgetController *)
{
    qWarning("MStyleSheet::style() is deprecated. Use public API from MTheme instead.");
    return NULL;
}

MStyle *MStyleSheetPrivate::style(const QList<const MStyleSheet *> &sheets,
                                  const QVector<ParentData> &parentsData,
                                  const QByteArray &parentStyleName,
                                  const QList<QByteArray> &styleMetaObjectHierarchy,
                                  const QByteArray &styleClassName,
                                  const QByteArray &objectName,
                                  const QByteArray &mode,
                                  const QByteArray &type,
                                  M::Orientation orientation)
{
    // TODO: check whether we can pass parentStyleName inside the
    // parentsData. It might enable us to support more cases in CSS if it is desired.

    if (!MClassFactory::instance()->styleMetaObject(styleClassName))
        return NULL;

    StyleSpec spec(styleClassName, styleMetaObjectHierarchy, objectName, mode, type, orientation);

    // Get the styling data that exists due to the parents in the scene.
    QList<MStyleSheetPrivate::SelectorInfo> sceneInfo;
    sceneInfo = MStyleSheetPrivate::getMatchingSelectorsWithParent(sheets, parentsData, parentStyleName, spec);
    spec.setParentInfo(&sceneInfo);

    // Look in the cache first.
    QByteArray identifier = spec.styleCacheKey();
    MStyle *style = MStyleSheetPrivate::StyleCache.value(identifier, NULL);
    if (style) {
        style->addReference();
        return style;
    }

    // If not yet cached, try to build a new style and add it to the cache.
    style = MStyleSheetPrivate::buildStyle(spec, sheets, parentsData, parentStyleName);
    if (style) {
        MStyleSheetPrivate::StyleCache.insert(identifier, style);
        MThemePrivate::addLeakedStyle(style, identifier);
    }

    return style;
}

void MStyleSheet::releaseStyle(const MStyle *style)
{
    // TODO: Optimize this later
    QHash<QByteArray, MStyle *>::iterator end = MStyleSheetPrivate::StyleCache.end();
    for (QHash<QByteArray, MStyle *>::iterator iterator = MStyleSheetPrivate::StyleCache.begin();
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

void MStyleSheet::cleanup(bool)
{
    // delete entry cache
    QHash<QByteArray, MStyleSheetPrivate::CacheEntry *>::iterator end = MStyleSheetPrivate::EntryCache.end();
    for (QHash<QByteArray, MStyleSheetPrivate::CacheEntry *>::iterator iterator = MStyleSheetPrivate::EntryCache.begin();
            iterator != end;
            ++iterator) {

        MStyleSheetPrivate::CacheEntry *entry = *iterator;
        qDeleteAll(*entry);
        delete entry;
    }
    MStyleSheetPrivate::EntryCache.clear();

    MStyleSheetPrivate::StyleCache.clear();
}

MStyleSheetPrivate::CacheEntry *MStyleSheetPrivate::buildCacheEntry(const QList<const MStyleSheet *>& sheets,
                                                                    const StyleSpec &spec,
                                                                    const QVector<ParentData> &parentsData,
                                                                    const QByteArray &parentStyleName)
{
    CacheEntry *entry = new CacheEntry;

    foreach(const MStyleSheet * sheet, sheets) {
        foreach(QSharedPointer<const MStyleSheetParser::StylesheetFileInfo> fi, sheet->fileInfoList()) {
            foreach(MStyleSheetSelector * selector, fi->selectors) {
                // mDebug("MStyleSheet") << "Searching style data for: " << cname << '#' << objectName;
                // mDebug("MStyleSheet") << "Comparing against" << selector->className() << '#' << selector->objectName();

                unsigned int parentPriority, classPriority;
                if (!MStyleSheetPrivate::matchParents(selector, parentsData, parentStyleName, parentPriority) ||
                    !spec.match(selector, classPriority)) {
                    continue;
                }

                // None of the early out tests failed, so we're happy with these settings, loop 'em through and copy them to list
                MAttributeList::const_iterator attributesEnd = selector->attributes()->constEnd();

                for (MAttributeList::const_iterator j = selector->attributes()->constBegin(); j != attributesEnd; ++j) {
                    MUniqueStringCache::Index propertyName = j.key();

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
                            existing->stylesheet = sheet;
                        }
                    } else {
                        // New settings, just add them to the hash
                        (*entry)[propertyName] = new MOriginContainer(j.value(),
                                selector,
                                classPriority,
                                parentPriority,
                                fi->filename,
                                sheet);
                    }
                }
            }
        }
    }

    return entry;
}

bool MStyleSheetPrivate::combine(MStyle *style, const CacheEntry &entry, const StyleSpec &spec)
{
    // we need to match data with parent information for the cache entry, this will
    // result to a combined list of attributes, which can be populated for the style

    // create copy of cache entry, we can't modify the original one for ovbious reasons
    CacheEntry data = entry;
    QList<MOriginContainer *> tempMOriginContainers;

    // match parent selectors to cached data
    foreach(const SelectorInfo & info, *spec.parentInfo) {

        if (!info.selector->attributes())
            continue;

        // check all the attributes of this selector against the cached entry
        foreach(MStyleSheetAttribute * attribute, *(info.selector->attributes())) {
            MOriginContainer *old = data.value(attribute->getNameID(), NULL);
            if (old && !isHigherPriority(old, info.selector, info.classPriority, info.parentPriority)) {
                    continue;
            }

            // override
            MOriginContainer *tempMOriginCont =   new MOriginContainer(attribute, info.selector,
                                                                       info.classPriority,
                                                                       info.parentPriority,
                                                                       info.filename, info.stylesheet);
            data[attribute->getNameID()] = tempMOriginCont;
            tempMOriginContainers.append(tempMOriginCont);
        }
    }

    bool result = true;

    // now loop through all properties and fill them to the style object
    const int propertyCount = style->metaObject()->propertyCount();
    for (int i = MStyle::staticMetaObject.propertyOffset(); i != propertyCount; ++i) {

        bool propertyInitialized = false;

        // find matching attribute from hash
        CacheEntry::iterator iterator = data.find(MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(style->metaObject()->property(i).name()));
        if (iterator != data.end()) {

            // get the attribute value
            MOriginContainer *container = *iterator;
            MStyleSheetAttribute *attribute = container->value;

            // erase this iterator from the data block, it is not needed anymore
            data.erase(iterator);

            if (!attribute->writeAttribute(container->filename, style, style->metaObject()->property(i), spec.orientation)) {
                qCritical("Failed to write attribute: %s to property %s. The stylesheet syntax might be invalid (%s:%s) in %s.",
                          attribute->getName().data(),
                          style->metaObject()->property(i).name(),
                          attribute->getName().data(),
                          attribute->getValue().data(),
                          container->filename.constData());

                result = container->stylesheet->syntaxMode() == MStyleSheet::RelaxedSyntax;
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
        QByteArray identifier = '(' + container->selector->className();
        if (!container->selector->classType().isEmpty())
            identifier += '[' + container->selector->classType() + ']';
        if (!container->selector->objectName().isEmpty())
            identifier += '#' + container->selector->objectName();
        if (container->selector->orientation() != MStyleSheetSelector::UndefinedOrientation)
            identifier += '.' + container->selector->orientation() == MStyleSheetSelector::PortraitOrientation
		          ? "Portrait" : "Landscape";
        if (!container->selector->mode().isEmpty())
            identifier += ':' + container->selector->mode();
        identifier += ')';

        mWarning("mstylesheet.cpp") << "unused attribute in" << container->filename << container->value->getName() << identifier;
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

    if (selector->objectNameID() != prev->selector->objectNameID()) {
        return selector->objectNameID() != MUniqueStringCache::EmptyStringIndex;
    }

    // Only other has mode, mode is more important than orientation
    if (selector->modeID() != prev->selector->modeID()) {
        return selector->modeID() != MUniqueStringCache::EmptyStringIndex;
    }

    // Other one has class type and another doesn't have it
    if (selector->classTypeID() != prev->selector->classTypeID()) {
        return selector->classTypeID() != MUniqueStringCache::EmptyStringIndex;
    }

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
    if (selector->orientation() != prev->selector->orientation())
	return selector->orientation() != MStyleSheetSelector::UndefinedOrientation;

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
int MStyleSheetPrivate::orderNumber(const QByteArray &n, const QByteArray &sn, const QByteArray &parentStyleName, const QList<QByteArray> &parentHierarchy)
{
    // Exit imediately if the style name does not match
    // parent style (object) name.
    if (!sn.isEmpty() && sn != parentStyleName)
        return -1;

    // Selectors class name may have "." in front of it, remove it
    if (n.startsWith('.')) {
        QByteArray realClassName = QByteArray::fromRawData(n.constData() + 1, n.length() - 1);
        return parentHierarchy.indexOf(realClassName);
    }

    return parentHierarchy.indexOf(n);
}

bool MStyleSheetPrivate::matchParent(const MStyleSheetSelector *selector,
                                     const QList<QByteArray> &parentHierarchy,
                                     const QByteArray &parentStyleName,
                                     unsigned int sceneOrder,
                                     unsigned int &parentPriority)
{
    parentPriority = MAKE_PRIORITY(0xffff, 0xffff);

    // Check whether the parent class derives from the given one
    int inheritanceOrder = MStyleSheetPrivate::orderNumber(selector->parentName(), selector->parentObjectName(), parentStyleName, parentHierarchy);
    if (inheritanceOrder != -1) {
        parentPriority = MAKE_PRIORITY(sceneOrder, inheritanceOrder);
        return true;
    }
    return false;
}

bool MStyleSheetPrivate::matchParents(const MStyleSheetSelector *selector,
                                      const QVector<ParentData> &parentsData,
                                      const QByteArray &parentStyleName,
                                      unsigned int &parentPriority)
{
    parentPriority = MAKE_PRIORITY(0xffff, 0xffff);

    if (selector->parentName().isEmpty()) {
        return true;
    }

    // This figures out whether the parent derives from the requested parent name
    int sceneOrder = 0;

    foreach (const ParentData &pd, parentsData) {
        if (matchParent(selector, pd.hierarchy, parentStyleName, sceneOrder, parentPriority))
            return true;

        if (selector->flags() & MStyleSheetSelector::Child)
            return false;

        sceneOrder++;
    }

    return false;
}

bool MStyleSheetPrivate::StyleSpec::match(const MStyleSheetSelector *selector,
                                          unsigned int &classPriority) const
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
            if (selector->className() != classHierarchy.at(0))
                return false;
        } else {
            // Early out: Make sure that we are a class of requested type or it's subclass
            order = MStyleSheetPrivate::orderNumber(selector->className(), QByteArray(), QByteArray(), classHierarchy);
            if (order == -1)
                return false;
        }
        // store inheritance order for later use
        classPriority = MAKE_PRIORITY(0xffff, order);
    }

    // Check whether the orientation matches
    if (selector->orientation() != MStyleSheetSelector::UndefinedOrientation &&
	orientation != static_cast<M::Orientation>(selector->orientation())) {
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

QList<MStyleSheetPrivate::SelectorInfo> MStyleSheetPrivate::getMatchingSelectorsWithParent(const QList<const MStyleSheet *> &sheets,
                                                                       const QVector<ParentData> &parentsData,
                                                                       const QByteArray &parentStyleName,
                                                                       const StyleSpec &spec)
{
    QList<SelectorInfo> results;
    unsigned int sceneOrder = 0;

    // Traverse from the most generic to the most specific parent sheets
    for (int i = parentsData.size() - 1; i >= 0; i--) {
        const ParentData &pd = parentsData[i];
        const QList<QByteArray> &thisParentHierarchy = pd.hierarchy;
        const QList<const MStyleSheet *> &thisParentSheets = pd.sheets;

        foreach (const MStyleSheet *sheet, thisParentSheets) {
            foreach(QSharedPointer<const MStyleSheetParser::StylesheetFileInfo> fi, sheet->fileInfoList()) {
                unsigned int parentPriority, classPriority;
                foreach (MStyleSheetSelector *selector, fi->parentSelectors) {
                    if (matchParent(selector, thisParentHierarchy, parentStyleName, sceneOrder, parentPriority) &&
                        spec.match(selector, classPriority)) {
                        // match found, store it to results list
                        SelectorInfo info;
                        info.filename = fi->filename;
                        info.selector = selector;
                        info.classPriority = classPriority;
                        info.parentPriority = parentPriority;
                        info.stylesheet = sheet;
                        results.append(info);
                    }
                }
            }
        }
        ++sceneOrder;
    }

    foreach (const MStyleSheet *sheet, sheets) {
        foreach (QSharedPointer<const MStyleSheetParser::StylesheetFileInfo> fi, sheet->fileInfoList()) {

            // loop trough all the selectors and find matching ones
            unsigned int parentPriority, classPriority;
            foreach (MStyleSheetSelector * selector, fi->parentSelectors) {
                if (matchParents(selector, parentsData, parentStyleName, parentPriority) &&
                    spec.match(selector, classPriority)) {

                    // match found, store it to results list
                    SelectorInfo info;
                    info.filename = fi->filename;
                    info.selector = selector;
                    info.classPriority = classPriority;
                    info.parentPriority = parentPriority;
                    info.stylesheet = sheet;
                    results.append(info);
                }
            }
        }
    }

    return results;
}

