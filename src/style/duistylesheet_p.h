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

#ifndef DUISTYLESHEET_P_H
#define DUISTYLESHEET_P_H

// Macros for handling parenting priority,
// upper 16 bits contain priority in scene graph
// lower 16 bits contain priority in inheritance chain
#define MAKE_PRIORITY(sceneOrder, inheritanceOrder)     ((sceneOrder << 16) | (inheritanceOrder & 0xffff));
#define EXTRACT_SCENEORDER(priority)                    (priority >> 16)
#define EXTRACT_INHERITANCEORDER(priority)              (priority & 0xffff)

#include <QObject>
#include <QString>
#include <QList>

class DuiOriginContainer;
class DuiStyleSheetSelector;
class DuiStyle;
class DuiWidgetController;

class DuiStyleSheetPrivate
{
public:
    struct SelectorInfo {
        QString filename;
        DuiStyleSheetSelector *selector;
        int classPriority;
        int parentPriority;
    };

    struct Key {
        Key(const QString &className, const QString &objectName, const QString &mode, const QString &type, const Dui::Orientation &orientation, const QList<SelectorInfo>* parentInfo) :
            className(className), objectName(objectName), mode(mode), type(type), orientation(orientation), parentInfo(parentInfo) {}

        QString uniqueId() const {
            // basic identifier without scene graph info
            QString orientationAsString((orientation == Dui::Landscape) ? "Landscape" : "Portrait");
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
        Dui::Orientation orientation;
        const QList<SelectorInfo>* parentInfo;
    };

    typedef QHash<QString, DuiOriginContainer *> CacheEntry;
    static QHash<QString, CacheEntry *> EntryCache;
    static QHash<QString, DuiStyle *> StyleCache;

    static CacheEntry *buildCacheEntry(const QList<const DuiStyleSheet *>& sheets,
                                       const QMetaObject &styleMetaObject,
                                       const QString &objectName,
                                       const QString &mode,
                                       const QString &type,
                                       const Dui::Orientation &orientation,
                                       const DuiWidgetController *parent);

    static bool combine(DuiStyle *style, const CacheEntry &entry, const QList<SelectorInfo>& parentInfo, const Dui::Orientation &orientation);

    static bool isHigherPriority(DuiOriginContainer *prev, DuiStyleSheetSelector *n, unsigned int classPriority, unsigned int parentPriority);

    static int orderNumber(const QString &n, const QMetaObject *mobj);

    static bool populateStyle(const QList<const DuiStyleSheet *>& sheets,
                              DuiStyle *style,
                              const QString &objectName,
                              const QString &mode,
                              const QString &type,
                              const Dui::Orientation &orientation,
                              const DuiWidgetController *parent,
                              const QList<SelectorInfo>& parentInfo);

    static bool match(DuiStyleSheetSelector *selector,
                      const QMetaObject &styleMetaObject,
                      const QString &objectName,
                      const QString &mode,
                      const QString &type,
                      const Dui::Orientation &orientation,
                      const DuiWidgetController *parent,
                      unsigned int &classPriority,
                      unsigned int &parentPriority);

    static void getMatchingSelectorsWithParent(const QList<const DuiStyleSheet *>& sheets,
            const QMetaObject &styleMetaObject,
            const QString &objectName,
            const QString &mode,
            const QString &type,
            const Dui::Orientation &orientation,
            const DuiWidgetController *parent,
            QList<SelectorInfo>& results);
};

#endif
