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

#ifndef MSTYLESHEET_P_H
#define MSTYLESHEET_P_H

// Macros for handling parenting priority,
// upper 16 bits contain priority in scene graph
// lower 16 bits contain priority in inheritance chain
#define MAKE_PRIORITY(sceneOrder, inheritanceOrder)     ((sceneOrder << 16) | (inheritanceOrder & 0xffff));
#define EXTRACT_SCENEORDER(priority)                    (priority >> 16)
#define EXTRACT_INHERITANCEORDER(priority)              (priority & 0xffff)

#include <QObject>
#include <QString>
#include <QList>

class MOriginContainer;
class MStyleSheetSelector;
class MStyle;
class MWidgetController;

class MStyleSheetPrivate
{
public:
    struct SelectorInfo {
        QByteArray filename;
        MStyleSheetSelector *selector;
        int classPriority;
        int parentPriority;
        const MStyleSheet *stylesheet;
    };

    // Aggregates the (non-parent) data that specifies which style we want to obtain
    struct StyleSpec {
        StyleSpec(const QByteArray &className, const QList<QByteArray> &classHierarchy, const QByteArray &objectName, const QByteArray &mode, const QByteArray &type, M::Orientation orientation) :
            className(className), classHierarchy(classHierarchy), objectName(objectName), mode(mode), type(type), orientation(orientation), parentInfo(0) {}

        void setParentInfo(const QList<SelectorInfo> *newParentInfo) {
            parentInfo = newParentInfo;
        }

        QByteArray entryCacheKey() const {
            if (!entryKey.isEmpty()) {
                return entryKey;
            }
            QByteArray orientationAsString((orientation == M::Landscape) ? "L" : "P");
            const_cast<StyleSpec*>(this)->entryKey = className + "[" + type + "]" + "#" + objectName + "." + orientationAsString + ":" + mode;
            return entryKey;
        }

        QByteArray styleCacheKey() const {
            QByteArray result = entryCacheKey();

            if (parentInfo && !parentInfo->isEmpty()) {
                result +=  + "{";
                for (QList<SelectorInfo>::const_iterator iterator = parentInfo->begin(); iterator != parentInfo->constEnd(); ++iterator) {
                    if (iterator != parentInfo->constBegin()) {
                        result += ',';
                    }
                    result += QByteArray::number((quintptr) iterator->selector, 16);
                }
                result += '}';
            }

            return result;
        }

        bool match(const MStyleSheetSelector *selector, unsigned int &classPriority) const;

        const QByteArray className;
        const QList<QByteArray> classHierarchy;

        const QByteArray objectName;
        const QByteArray mode;
        const QByteArray type;
        M::Orientation orientation;

        QByteArray entryKey;

        const QList<SelectorInfo> *parentInfo;
    };

    // Basic structure used to aggregate parent data of the style we want to obtain
    struct ParentData {
        QList<QByteArray> hierarchy;
        QList<const MStyleSheet *> sheets;
    };

    static QVector<ParentData> extractParentsData(const MWidgetController *parent);

    static MStyle *style(const QList<const MStyleSheet *> &sheets,
                         const QVector<ParentData> &parentsData,
                         const QByteArray &parentStyleName,
                         const QList<QByteArray> &styleMetaObjectHierarchy,
                         const QByteArray &styleClassName,
                         const QByteArray &objectName,
                         const QByteArray &mode,
                         const QByteArray &type,
                         M::Orientation orientation);

    typedef QHash<QByteArray, MOriginContainer *> CacheEntry;
    static QHash<QByteArray, CacheEntry *> EntryCache;
    static QHash<QByteArray, MStyle *> StyleCache;

    static CacheEntry *buildCacheEntry(const QList<const MStyleSheet *> &sheets,
                                       const StyleSpec &spec,
                                       const QVector<ParentData> &parentsData,
                                       const QByteArray &parentStyleName);

    static bool combine(MStyle *style, const CacheEntry &entry, const StyleSpec &spec);

    static bool isHigherPriority(MOriginContainer *prev, MStyleSheetSelector *n, unsigned int classPriority, unsigned int parentPriority);

    static int orderNumber(const QByteArray &n, const QByteArray &sn, const QByteArray &parentStyleName, const QList<QByteArray> &parentHierarchy);

    static MStyle *buildStyle(const StyleSpec &spec,
                              const QList<const MStyleSheet *> &sheets,
                              const QVector<ParentData> &parentsData,
                              const QByteArray &parentStyleName);

    static bool matchParent(const MStyleSheetSelector *selector,
                            const QList<QByteArray> &parentHierarchy,
                            const QByteArray &parentStyleName,
                            unsigned int sceneOrder,
                            unsigned int &parentPriority);

    static bool matchParents(const MStyleSheetSelector *selector,
                             const QVector<ParentData> &parentsData,
                             const QByteArray &parentStyleName,
                             unsigned int &parentPriority);

    static QList<SelectorInfo> getMatchingSelectorsWithParent(const QList<const MStyleSheet *> &sheets,
                                                              const QVector<ParentData> &parentsData,
                                                              const QByteArray &parentStyleName,
                                                              const StyleSpec &spec);
};

#endif
