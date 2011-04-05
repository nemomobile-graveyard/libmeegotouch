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

#include "muniquestringcache.h"
#include "mnamespace.h"

#include <QObject>
#include <QString>
#include <QList>
#include <QHash>

class MStyleSheetSelector;
class MStyle;
class MWidgetController;
class MStyleSheet;

class MStyleSheetPrivate
{
public:
    struct SelectorInfo {
        SelectorInfo(const MStyleSheetSelector *selector,
                     int classPriority,
                     int parentPriority,
                     const MStyleSheet *stylesheet) :
            selector(selector),
            classPriority(classPriority),
            parentPriority(parentPriority),
            stylesheet(stylesheet)
        {}
        const MStyleSheetSelector *selector;
        int classPriority;
        int parentPriority;
        const MStyleSheet *stylesheet;
    };

    struct SelectorInfoList {
        bool orientationDependent;
        QList<QSharedPointer<SelectorInfo> > selectorInfos;
    };

    // Aggregates the (non-parent) data that specifies which style we want to obtain
    struct StyleSpec {
        StyleSpec(const QByteArray &className, const QList<QByteArray> &classHierarchy, const QByteArray &objectName, const QByteArray &mode, const QByteArray &type, M::Orientation orientation) :
            className(className), classHierarchy(classHierarchy), objectName(objectName), mode(mode), type(type), orientation(orientation), parentInfo(0) {}

        void setParentInfo(const SelectorInfoList *newParentInfo) {
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

            if (parentInfo && !parentInfo->selectorInfos.isEmpty()) {
                result +=  + "{";
                for (QList<QSharedPointer<SelectorInfo> >::const_iterator iterator = parentInfo->selectorInfos.begin(); iterator != parentInfo->selectorInfos.constEnd(); ++iterator) {
                    if (iterator != parentInfo->selectorInfos.constBegin()) {
                        result += ',';
                    }
                    result += QByteArray::number((quintptr) (*iterator)->selector, 16);
                }
                result += '}';
            }

            return result;
        }

        bool matchOrientationIndependent(const MStyleSheetSelector *selector, unsigned int &classPriority) const;

        const QByteArray className;
        const QList<QByteArray> classHierarchy;

        const QByteArray objectName;
        const QByteArray mode;
        const QByteArray type;
        M::Orientation orientation;

        QByteArray entryKey;

        const SelectorInfoList *parentInfo;
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

    static QHash<QByteArray, SelectorInfoList*> EntryCache;

    static SelectorInfoList *buildSelectorInfoList(const QList<const MStyleSheet *> &sheets,
                                       const StyleSpec &spec,
                                       const QVector<ParentData> &parentsData,
                                       const QByteArray &parentStyleName);

    static bool combine(MStyle *style, const SelectorInfoList &entry, const StyleSpec &spec);

    static bool isHigherPriority(const QSharedPointer<SelectorInfo> &prev, const QSharedPointer<SelectorInfo> &next);

    static int orderNumber(const QLatin1String &n, const QLatin1String &sn, const QByteArray &parentStyleName, const QList<QByteArray> &parentHierarchy);

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

    static MStyleSheetPrivate::SelectorInfoList getMatchingSelectorsWithParent(const QList<const MStyleSheet *> &sheets,
                                                              const QVector<ParentData> &parentsData,
                                                              const QByteArray &parentStyleName,
                                                              const StyleSpec &spec);

    static void matchOrientationDependent(QList<QSharedPointer<SelectorInfo> > *selectorInfos,
                                          bool *orientationDependent, M::Orientation targetOriention);

    struct StyleCache {
        ~StyleCache();
        typedef QHash<QByteArray,MStyle *> StyleHash;
        QHash<QByteArray, StyleHash> styles;
    };
};

#endif
