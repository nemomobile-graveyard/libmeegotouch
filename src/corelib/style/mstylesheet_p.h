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

    struct Key {
        Key(const QString &className, const QString &objectName, const QString &mode, const QString &type, M::Orientation orientation, const QList<SelectorInfo>* parentInfo) :
            className(className), objectName(objectName), mode(mode), type(type), orientation(orientation), parentInfo(parentInfo) {}

        QString uniqueId() const {
            // basic identifier without scene graph info
            QString orientationAsString((orientation == M::Landscape) ? "Landscape" : "Portrait");
            QString id = className + "[" + type + "]" + "#" + objectName + "." + orientationAsString + ":" + mode;

            if (parentInfo) {
                // scene graph info
                id +=  + "{";
                for (QList<SelectorInfo>::const_iterator iterator = parentInfo->begin(); iterator != parentInfo->constEnd(); ++iterator) {
                    if (iterator != parentInfo->constBegin()) {
                        id += ',';
                    }
                    id += QString::number((quintptr) iterator->selector, 16);
                }
                id += '}';
            }
            return id;
        }

        QString className, objectName, mode, type;
        M::Orientation orientation;
        const QList<SelectorInfo>* parentInfo;
    };

    typedef QHash<QString, MOriginContainer *> CacheEntry;
    static QHash<QString, CacheEntry *> EntryCache;
    static QHash<QString, MStyle *> StyleCache;

    static CacheEntry *buildCacheEntry(const QList<const MStyleSheet *>& sheets,
                                       const QMetaObject &styleMetaObject,
                                       const QString &objectName,
                                       const QString &mode,
                                       const QString &type,
                                       M::Orientation orientation,
                                       const MWidgetController *parent);

    static bool combine(MStyle *style, const CacheEntry &entry, const QList<SelectorInfo>& parentInfo, M::Orientation orientation);

    static bool isHigherPriority(MOriginContainer *prev, MStyleSheetSelector *n, unsigned int classPriority, unsigned int parentPriority);

    static int orderNumber(const QString &n, const QMetaObject *mobj);

    static bool populateStyle(const QList<const MStyleSheet *>& sheets,
                              MStyle *style,
                              const QString &objectName,
                              const QString &mode,
                              const QString &type,
                              M::Orientation orientation,
                              const MWidgetController *parent,
                              const QList<SelectorInfo>& parentInfo);

    static bool matchParent(MStyleSheetSelector *selector,
                            const QMetaObject* mobj,
                            unsigned int sceneOrder,
                            unsigned int &parentPriority);

    static bool matchParents(MStyleSheetSelector *selector,
                             const MWidgetController *parent,
                             unsigned int &parentPriority);

    static bool match(MStyleSheetSelector *selector,
                      const QMetaObject &styleMetaObject,
                      const QString &objectName,
                      const QString &mode,
                      const QString &type,
                      M::Orientation orientation,
                      unsigned int &classPriority);

    static void getMatchingSelectorsWithParent(const QList<const MStyleSheet *>& sheets,
            const QMetaObject &styleMetaObject,
            const QList<QPair<const QMetaObject*, QList<const MStyleSheet*> > > parentsSheets,
            const QString &objectName,
            const QString &mode,
            const QString &type,
            M::Orientation orientation,
            const MWidgetController *parent,
            QList<SelectorInfo>& results);
};

#endif
