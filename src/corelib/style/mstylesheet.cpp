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
#include "mcomponentdata.h"
#include "mcomponentdata_p.h"

Q_DECLARE_METATYPE(const QPixmap *)
namespace {
void releaseAllocatedResourcesFromStyle(const MStyle *style)
{
    // TODO: Fix this in mtheme so, that the pixmaps are not pointers
    // and therefore we don't need to call release at all.
    const int propertyCount = style->metaObject()->propertyCount();
    for (int i = 0; i < propertyCount; ++i) {
        const QMetaProperty &property = style->metaObject()->property(i);
        if (property.isReadable()) {
            if (property.userType() == qMetaTypeId<const QPixmap *>()) {
                MTheme::releasePixmap(qvariant_cast<const QPixmap *>(property.read(style)));
            } else if (property.userType() == qMetaTypeId<const MScalableImage *>()) {
                MTheme::releaseScalableImage(qvariant_cast<const MScalableImage *>(property.read(style)));
            }
        }
    }
}

Q_GLOBAL_STATIC(MStyleSheetPrivate::StyleCache, styleCache)
}

void mMessageHandler(QtMsgType type, const char *msg);

MStyleSheetPrivate::StyleCache::~StyleCache()
{
    // free all styles which have been released after the d'tor of MTheme
    // MStyleSheet::DoNotReleaseResources is passed as most parts of Qt are
    // already destroyed and freeing resources would lead to a crash
    MStyleSheet::deleteStylesWithoutReference();

    // as LeakedStyles is a static class the method handler could not be valid
    // anymore at this point. work around this problem by creating a new one
    qInstallMsgHandler(mMessageHandler);

    foreach(const StyleHash &styleHash, styles) {
        QHash<QByteArray, MStyle *>::const_iterator end = styleHash.end();
        for (QHash<QByteArray, MStyle *>::const_iterator iterator = styleHash.begin();
             iterator != end;
             ++iterator) {
            QByteArray id = iterator.key();
            MStyle *leak = iterator.value();
            mWarning("mtheme.cpp") << "Style:" << id << "not released!" << "refcount:" << leak->references();
        }
    }
}

QHash<QByteArray, MStyleSheetPrivate::SelectorInfoList *> MStyleSheetPrivate::EntryCache;
//QHash<QByteArray, MStyle *> MStyleSheetPrivate::StyleCache;

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

    // We keep a second cache, the EntryCache, which stores a list of all matching
    // selectors without parents sorted by priority
    QByteArray identifier = spec.entryCacheKey();
    MStyleSheetPrivate::SelectorInfoList *entry = MStyleSheetPrivate::EntryCache.value(identifier, NULL);

    if (!entry) {
        entry = MStyleSheetPrivate::buildSelectorInfoList(sheets, spec, parentsData, parentStyleName);
        MStyleSheetPrivate::EntryCache.insert(identifier, entry);
    }

    style->setOrientationDependent(spec.parentInfo->orientationDependent || entry->orientationDependent);

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

            while (mobj->className() != QObject::staticMetaObject.className() &&
                   mobj->className() != QGraphicsWidget::staticMetaObject.className()) {
                pd.hierarchy.append(mobj->className());
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
    SelectorInfoList sceneInfo;
    sceneInfo = MStyleSheetPrivate::getMatchingSelectorsWithParent(sheets, parentsData, parentStyleName, spec);
    spec.setParentInfo(&sceneInfo);

    // Look in the cache first.
    QByteArray identifier = spec.styleCacheKey();
    QHash<QByteArray, StyleCache::StyleHash>::iterator styleHashIt = styleCache()->styles.find(spec.className);
    if (styleHashIt != styleCache()->styles.end()) {
        MStyle *style = styleHashIt->value(identifier, NULL);
        if (style) {
            style->addReference();
            return style;
        }
    }

    // If not yet cached, try to build a new style and add it to the cache.
    MStyle *style = MStyleSheetPrivate::buildStyle(spec, sheets, parentsData, parentStyleName);
    if (style) {
        styleCache()->styles[spec.className].insert(identifier, style);
    }

    return style;
}

void MStyleSheet::releaseStyle(const MStyle *style)
{
    // TODO: Optimize this later
    foreach(const MStyleSheetPrivate::StyleCache::StyleHash &styleHash, styleCache()->styles) {
        QHash<QByteArray, MStyle *>::const_iterator end = styleHash.end();
        for (QHash<QByteArray, MStyle *>::const_iterator iterator = styleHash.begin();
             iterator != end;
             ++iterator) {
            MStyle *cached = *iterator;
            if (style == cached) {
                int count = cached->removeReference();
                if (count < 0) {
                    mWarning("MStyleSheet::releaseStyle") << "Reference count for" << iterator.key() << "is below 0. Something is going wrong.";
                }
                return;
            }
        }
    }

    // unknown style object, or cache is disabled
    const_cast<MStyle *>(style)->removeReference();
}

void MStyleSheet::deleteStylesWithoutReference()
{
    typedef MStyleSheetPrivate::StyleCache::StyleHash StyleHash;
    QHash<QByteArray, StyleHash>::iterator end = styleCache()->styles.end();
    for (QHash<QByteArray, StyleHash>::iterator styleHashIt = styleCache()->styles.begin();
         styleHashIt != end;
         ++styleHashIt) {
        QMutableHashIterator<QByteArray, MStyle *> it(*styleHashIt);
        while (it.hasNext()) {
            MStyle *style = it.next().value();
            if (style->references() <= 0) {
                if (MComponentData::instance() && MComponentData::instance()->d_ptr->theme) {
                    releaseAllocatedResourcesFromStyle(style);
                }
                delete style;
                it.remove();
            }
        }
    }
}

void MStyleSheet::deleteStylesFromStyleCreator(const QByteArray & styleClassName)
{
    if (!styleCache()) {
        // early out if the style cache is already destroyed
        // ~StyleCache() will do the needed checks
        return;
    }

    typedef MStyleSheetPrivate::StyleCache::StyleHash StyleHash;
    QHash<QByteArray, StyleHash>::iterator styleHashIt = styleCache()->styles.find(styleClassName);
    if (styleHashIt != styleCache()->styles.end()) {
        QMutableHashIterator<QByteArray, MStyle *> it(*styleHashIt);
        while (it.hasNext()) {
            MStyle *style = it.next().value();
            if (style->references() > 0) {
                mWarning("MStyleSheet::deleteStylesFromStyleCreator") << "Removing style creator for" << styleClassName
                    << "but style" << it.key() << "has still" << style->references() << "references. This can lead to a crash if style is used somewhere!";
            }
            if (MComponentData::instance() && MComponentData::instance()->d_ptr->theme) {
                releaseAllocatedResourcesFromStyle(style);
            }
            delete style;
            it.remove();
        }
    }
}

void MStyleSheet::cleanup(bool)
{
    // delete entry cache
    QHash<QByteArray, MStyleSheetPrivate::SelectorInfoList *>::iterator end = MStyleSheetPrivate::EntryCache.end();
    for (QHash<QByteArray, MStyleSheetPrivate::SelectorInfoList *>::iterator iterator = MStyleSheetPrivate::EntryCache.begin();
            iterator != end;
            ++iterator) {

        MStyleSheetPrivate::SelectorInfoList *entry = *iterator;
        delete entry;
    }
    MStyleSheetPrivate::EntryCache.clear();

    deleteStylesWithoutReference();
}

MStyleSheetPrivate::SelectorInfoList *MStyleSheetPrivate::buildSelectorInfoList(const QList<const MStyleSheet *>& sheets,
                                                                    const StyleSpec &spec,
                                                                    const QVector<ParentData> &parentsData,
                                                                    const QByteArray &parentStyleName)
{
    SelectorInfoList *entry = new SelectorInfoList;

    entry->orientationDependent = false;
    foreach(const MStyleSheet * sheet, sheets) {
        QList<const MStyleSheetSelector*> selectors = sheet->selectorList(spec);

        foreach(const MStyleSheetSelector * selector, selectors) {
            unsigned int parentPriority = 0xffffffff;
            unsigned int classPriority = 0xffffffff;
            if (!MStyleSheetPrivate::matchParents(selector, parentsData, parentStyleName, parentPriority) ||
                    !spec.matchOrientationIndependent(selector, classPriority)) {
                continue;
            }

            QSharedPointer<SelectorInfo> info(new SelectorInfo(selector, classPriority, parentPriority, sheet));
            entry->selectorInfos.push_back(info);
        }
    }

    matchOrientationDependent(&entry->selectorInfos, &entry->orientationDependent, spec.orientation);

    qStableSort(entry->selectorInfos.begin(), entry->selectorInfos.end(), MStyleSheetPrivate::isHigherPriority);

    return entry;
}

bool MStyleSheetPrivate::combine(MStyle *style, const SelectorInfoList &entry, const StyleSpec &spec)
{
    QList<QSharedPointer<SelectorInfo> > candidates = entry.selectorInfos;
    // The selectors without parent are already sorted. As selectors with parent
    // very likely have a high priority we search for a place to insert them from the back
    // of the parent independent selectors.
    QListIterator<QSharedPointer<SelectorInfo> > parentIt(spec.parentInfo->selectorInfos);
    parentIt.toBack();
    while (parentIt.hasPrevious()) {
        const QSharedPointer<SelectorInfo> &newSelectorInfo = parentIt.previous();
        QMutableListIterator<QSharedPointer<SelectorInfo> > candidateIt(candidates);
        candidateIt.toBack();
        while (candidateIt.hasPrevious()) {
            QSharedPointer<SelectorInfo> info = candidateIt.previous();
            if (isHigherPriority(info, newSelectorInfo)) {
                candidateIt.next();
                candidateIt.insert(newSelectorInfo);
                break;
            }
        }
        if (!candidateIt.hasPrevious()) {
            candidates.push_front(newSelectorInfo);
        }
    }

    bool result = true;

    // now loop through all properties and fill them to the style object
    const int propertyCount = style->metaObject()->propertyCount();
    for (int i = MStyle::staticMetaObject.propertyOffset(); i != propertyCount; ++i) {
        QMetaProperty property = style->metaObject()->property(i);
        QByteArray propertyName = QByteArray::fromRawData(property.name(), qstrlen(property.name()));
        MUniqueStringCache::Index propertyIndex = MStyleSheetParser::stringCacheWithReverseLookup()->stringToIndex(propertyName);
        bool propertyInitialized = false;

        // search for attribute in selectors based on their priority
        QListIterator<QSharedPointer<SelectorInfo> > it(candidates);
        it.toBack();
        while (it.hasPrevious()) {
            QSharedPointer<SelectorInfo> info = it.previous();
            if (const MStyleSheetAttribute *attribute = info->selector->attributeByName(propertyIndex)) {

                if (!attribute->writeAttribute(info->selector->filenameID(), style, property, spec.orientation)) {
                    qCritical("Failed to write attribute: %s to property %s. The stylesheet syntax might be invalid (%s:%s) in %s.",
                              attribute->getName().latin1(),
                              property.name(),
                              attribute->getName().latin1(),
                              attribute->getValue().latin1(),
                              info->selector->filename().latin1());

                    result = info->stylesheet->syntaxMode() == MStyleSheet::RelaxedSyntax;
                } else {
                    propertyInitialized = true;
                }
                break;
            }
        }
        if (propertyInitialized == false) {
            mWarning("mstylesheet.cpp") << "Property" <<
                                           style->metaObject()->property(i).name() <<
                                           "was left uninitialized in" << style->metaObject()->className();
        }
    }

    return result;
}

bool MStyleSheetPrivate::isHigherPriority(const QSharedPointer<SelectorInfo> &prev,
        const QSharedPointer<SelectorInfo> &next)
{
    // At this stage we either have a correct object name or we don't have object name at all.
    // So, select the one which has it. If both have same name or neither one has it, go further.

    if (next->selector->objectNameID() != prev->selector->objectNameID()) {
        return next->selector->objectNameID() != MUniqueStringCache::EmptyStringIndex;
    }

    // Only other has mode, mode is more important than orientation
    if (next->selector->modeID() != prev->selector->modeID()) {
        return next->selector->modeID() != MUniqueStringCache::EmptyStringIndex;
    }

    // Other one has class type and another doesn't have it
    if (next->selector->classTypeID() != prev->selector->classTypeID()) {
        return next->selector->classTypeID() != MUniqueStringCache::EmptyStringIndex;
    }

    // The closer one in the scene chain has more priority, 0xffff means no match
    unsigned int sceneOrder = EXTRACT_SCENEORDER(next->parentPriority);
    unsigned int inheritanceOrder = EXTRACT_INHERITANCEORDER(next->parentPriority);
    unsigned int previousSceneOrder = EXTRACT_SCENEORDER(prev->parentPriority);
    unsigned int previousInheritanceOrder = EXTRACT_INHERITANCEORDER(prev->parentPriority);

    // scene order
    if ((sceneOrder != 0xffff) || (previousSceneOrder != 0xffff)) {
        if (sceneOrder != previousSceneOrder) {
            return sceneOrder < previousSceneOrder;
        }
        // direct child ?
        else if (((prev->selector->flags() & MStyleSheetSelector::Child) != (next->selector->flags() & MStyleSheetSelector::Child)) && (inheritanceOrder == previousInheritanceOrder)) {
            return (next->selector->flags() & MStyleSheetSelector::Child) ? true : false;
        }
    }

    // inheritance order
    if ((inheritanceOrder != 0xffff) || (previousInheritanceOrder != 0xffff)) {
        if (inheritanceOrder != previousInheritanceOrder) {
            return inheritanceOrder < previousInheritanceOrder;
        }
    }

    // check if parent name is set
    if (next->selector->parentObjectName() != prev->selector->parentObjectName()) {
        return next->selector->parentObjectNameID() != MUniqueStringCache::EmptyStringIndex;
    }

    // Only other has orientation
    if (next->selector->orientation() != prev->selector->orientation())
    return next->selector->orientation() != MStyleSheetSelector::UndefinedOrientation;

    // Check the level of inheritance in class hierarchy for the existing entry for both entries
    inheritanceOrder = EXTRACT_INHERITANCEORDER(next->classPriority);
    previousInheritanceOrder = EXTRACT_INHERITANCEORDER(prev->classPriority);

    // Check if the new one is closer to the requested class type in class hierarchy
    if (inheritanceOrder < previousInheritanceOrder)
        return true;
    if (inheritanceOrder > previousInheritanceOrder)
        return false;

    // -> Same level of inheritance

    // These are completely equal, existing one has higher priority
    return false;
}

// calculates "order" number (how far is in superclass inheritance tree)
int MStyleSheetPrivate::orderNumber(const QLatin1String &n, const QLatin1String &sn, const QByteArray &parentStyleName, const QList<QByteArray> &parentHierarchy)
{
    // Exit imediately if the style name does not match
    // parent style (object) name.
    if (sn != QLatin1String("") && sn != parentStyleName.constData())
        return -1;

    // Selectors class name may have "." in front of it, remove it
    if (n != QLatin1String("") && *(n.latin1()) == '.') {
        QByteArray realClassName = QByteArray::fromRawData(n.latin1() + 1, strlen(n.latin1()) - 1);
        return parentHierarchy.indexOf(realClassName);
    }

    return parentHierarchy.indexOf(n.latin1());
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

    if (selector->parentNameID() == MUniqueStringCache::EmptyStringIndex) {
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

bool MStyleSheetPrivate::StyleSpec::matchOrientationIndependent(const MStyleSheetSelector *selector,
                                          unsigned int &classPriority) const
{
    // Initialize
    classPriority = MAKE_PRIORITY(0xffff, 0xffff);

    // Check whether the class name matches
    if (selector->classNameID() != MUniqueStringCache::EmptyStringIndex) {
        int order = 0;
        // Check if the class name contains '.' as a first letter, if it does it means that these are not for subclasses
        if (selector->flags() & MStyleSheetSelector::ExactClassMatch) {
            if (selector->className() != classHierarchy.at(0).constData()) {
                return false;
            }
        } else {
            // TODO: MStyleSheetSelectorTree::partiallyMatchingSelectors() already knows the order, no need to determine it again
            order = MStyleSheetPrivate::orderNumber(selector->className(), QLatin1String(""), QByteArray(), classHierarchy);
        }
        // store inheritance order for later use
        classPriority = MAKE_PRIORITY(0xffff, order);
    }

    // Check whether the type matches
    if (selector->classTypeID() != MUniqueStringCache::EmptyStringIndex && type.constData() != selector->classType()) {
        return false;
    }

    // Check whether the mode matches
    if (selector->modeID() != MUniqueStringCache::EmptyStringIndex&& mode.constData() != selector->mode()) {
        return false;
    }

    return true;
}

MStyleSheetPrivate::SelectorInfoList MStyleSheetPrivate::getMatchingSelectorsWithParent(const QList<const MStyleSheet *> &sheets,
                                                                       const QVector<ParentData> &parentsData,
                                                                       const QByteArray &parentStyleName,
                                                                       const StyleSpec &spec)
{
    SelectorInfoList results;
    unsigned int sceneOrder = 0;

    // Traverse from the most generic to the most specific parent sheets
    for (int i = parentsData.size() - 1; i >= 0; i--) {
        const ParentData &pd = parentsData[i];
        const QList<QByteArray> &thisParentHierarchy = pd.hierarchy;
        const QList<const MStyleSheet *> &thisParentSheets = pd.sheets;

        foreach (const MStyleSheet *sheet, thisParentSheets) {
            unsigned int parentPriority, classPriority;
            const QList<const MStyleSheetSelector*> selectors = sheet->parentSelectorList(spec);
            foreach (const MStyleSheetSelector *selector, selectors) {
                if (matchParent(selector, thisParentHierarchy, parentStyleName, sceneOrder, parentPriority) &&
                        spec.matchOrientationIndependent(selector, classPriority)) {
                    // match found, store it to results list
                    QSharedPointer<SelectorInfo> info(new SelectorInfo(selector, classPriority, parentPriority, sheet));
                    results.selectorInfos.append(info);
                }
            }
        }
        ++sceneOrder;
    }

    foreach (const MStyleSheet *sheet, sheets) {
        // loop trough all the selectors and find matching ones
        unsigned int parentPriority, classPriority;
        const QList<const MStyleSheetSelector*> selectors = sheet->parentSelectorList(spec);
        foreach (const MStyleSheetSelector *selector, selectors) {
            if (matchParents(selector, parentsData, parentStyleName, parentPriority) &&
                    spec.matchOrientationIndependent(selector, classPriority)) {
                // match found, store it to results list
                QSharedPointer<SelectorInfo> info(new SelectorInfo(selector, classPriority, parentPriority, sheet));
                results.selectorInfos.append(info);
            }
        }
    }

    results.orientationDependent = false;
    matchOrientationDependent(&results.selectorInfos, &results.orientationDependent, spec.orientation);

    return results;
}

void MStyleSheetPrivate::matchOrientationDependent(QList<QSharedPointer<SelectorInfo> > *selectorInfos,
                                                   bool *orientationDependent, M::Orientation targetOriention) {
    // check whether the style will be orientation dependent and remove all selectors
    // with the wrong orientation
    QMutableListIterator<QSharedPointer<SelectorInfo> > it(*selectorInfos);
    while (it.hasNext())
    {
        QSharedPointer<SelectorInfo> selectorInfo = it.next();
        if (selectorInfo->selector->orientation() != MStyleSheetSelector::UndefinedOrientation) {
            if (static_cast<M::Orientation>(selectorInfo->selector->orientation()) != targetOriention) {
                it.remove();
            }
            *orientationDependent = true;
        }
    }
}
