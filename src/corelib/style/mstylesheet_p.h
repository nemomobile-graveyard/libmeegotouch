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
        QString filename;
        MStyleSheetSelector *selector;
        int classPriority;
        int parentPriority;
    };

    // Aggregates the (non-parent) data that specifies which style we want to obtain
    struct StyleSpec {
        StyleSpec(const QString &className, const QStringList &classHierarchy, const QString &objectName, const QString &mode, const QString &type, M::Orientation orientation) :
            className(className), classHierarchy(classHierarchy), objectName(objectName), mode(mode), type(type), orientation(orientation), parentInfo(0) {}

        void setParentInfo(const QList<SelectorInfo> *newParentInfo) {
            parentInfo = newParentInfo;
        }

        QString entryCacheKey() const {
            QString orientationAsString((orientation == M::Landscape) ? "Landscape" : "Portrait");
            QString result = className + "[" + type + "]" + "#" + objectName + "." + orientationAsString + ":" + mode;
            return result;
        }

        QString styleCacheKey() const {
            QString result = entryCacheKey();

            if (parentInfo) {
                result +=  + "{";
                for (QList<SelectorInfo>::const_iterator iterator = parentInfo->begin(); iterator != parentInfo->constEnd(); ++iterator) {
                    if (iterator != parentInfo->constBegin()) {
                        result += ',';
                    }
                    result += QString::number((quintptr) iterator->selector, 16);
                }
                result += '}';
            }

            return result;
        }

        bool match(const MStyleSheetSelector *selector, unsigned int &classPriority) const;

        const QString className;
        const QStringList classHierarchy;

        const QString objectName;
        const QString mode;
        const QString type;
        M::Orientation orientation;

        const QList<SelectorInfo> *parentInfo;
    };

    // Basic structure used to aggregate parent data of the style we want to obtain
    struct ParentData {
        QStringList hierarchy;
        QList<const MStyleSheet *> sheets;
    };

    static QVector<ParentData> extractParentsData(const MWidgetController *parent);

    static MStyle *style(const QList<const MStyleSheet *> &sheets,
                         const QVector<ParentData> &parentsData,
                         const QString &parentStyleName,
                         const QStringList &styleMetaObjectHierarchy,
                         const QString &styleClassName,
                         const QString &objectName,
                         const QString &mode,
                         const QString &type,
                         M::Orientation orientation);

    typedef QHash<QString, MOriginContainer *> CacheEntry;
    static QHash<QString, CacheEntry *> EntryCache;
    static QHash<QString, MStyle *> StyleCache;

    static CacheEntry *buildCacheEntry(const QList<const MStyleSheet *> &sheets,
                                       const StyleSpec &spec,
                                       const QVector<ParentData> &parentsData,
                                       const QString &parentStyleName);

    static bool combine(MStyle *style, const CacheEntry &entry, const StyleSpec &spec);

    static bool isHigherPriority(MOriginContainer *prev, MStyleSheetSelector *n, unsigned int classPriority, unsigned int parentPriority);

    static int orderNumber(const QString &n, const QString &sn, const QString &parentStyleName, const QStringList &parentHierarchy);

    static MStyle *buildStyle(const StyleSpec &spec,
                              const QList<const MStyleSheet *> &sheets,
                              const QVector<ParentData> &parentsData,
                              const QString &parentStyleName);

    static bool matchParent(const MStyleSheetSelector *selector,
                            const QStringList &parentHierarchy,
                            const QString &parentStyleName,
                            unsigned int sceneOrder,
                            unsigned int &parentPriority);

    static bool matchParents(const MStyleSheetSelector *selector,
                             const QVector<ParentData> &parentsData,
                             const QString &parentStyleName,
                             unsigned int &parentPriority);

    static QList<SelectorInfo> getMatchingSelectorsWithParent(const QList<const MStyleSheet *> &sheets,
                                                              const QVector<ParentData> &parentsData,
                                                              const QString &parentStyleName,
                                                              const StyleSpec &spec);
};

#endif
