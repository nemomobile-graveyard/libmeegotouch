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

#ifndef MSTYLE_H
#define MSTYLE_H

#include <QObject>
#include "mexport.h"

// style macro
#define M_STYLE(CLASS) \
    public: \
    CLASS(); \
    protected: \
    virtual ~CLASS(); \
    CLASS(class CLASS##Data* data); \
    class CLASS##Data *data; \
    private:

// style macro for internal styles
#define M_STYLE_INTERNAL(CLASS) \
    public: \
    CLASS(); \
    virtual ~CLASS(); \
    protected: \
    CLASS(class CLASS##Data* data); \
    private:

// style attribute macro
#define M_STYLE_ATTRIBUTE(TYPE, NAME, CAMELNAME) \
    Q_PROPERTY(TYPE NAME READ NAME WRITE set##CAMELNAME) \
    public: \
    const TYPE& NAME() const; \
    void set##CAMELNAME(const TYPE& NAME); \
    private:

// style attribute macro for pointers
#define M_STYLE_PTR_ATTRIBUTE(TYPE, NAME, CAMELNAME) \
    Q_PROPERTY(TYPE NAME READ NAME WRITE set##CAMELNAME) \
    public: \
    const TYPE NAME() const; \
    void set##CAMELNAME(const TYPE NAME); \
    private:

// style container macro
// TODO: remove the dummy method below once we are allowed to break
// the ABI. it just exists to make sure the inlined operator->()
// is also exported as symbol
#define M_STYLE_CONTAINER(STYLE_CLASS) \
    public: \
    STYLE_CLASS##Container(); \
    virtual ~STYLE_CLASS##Container(); \
    const STYLE_CLASS* operator->() const { return static_cast<const STYLE_CLASS*>(currentStyle()); } \
    protected: \
    STYLE_CLASS##Container(class STYLE_CLASS##ContainerPrivate* dd); \
    virtual const char* styleType() const; \
    class STYLE_CLASS##ContainerPrivate * const d_ptr; \
    typedef const STYLE_CLASS* (STYLE_CLASS##Container::*dummyNeverToBeUsedPtr)() const; \
    dummyNeverToBeUsedPtr _dummyNeverToBeUsed(); \
    private: \
    Q_DECLARE_PRIVATE(STYLE_CLASS##Container)

// style container macro for internal styles
// TODO: remove the dummy method below once we are allowed to break
// the ABI. it just exists to make sure the inlined operator->()
// is also exported as symbo
#define M_STYLE_CONTAINER_INTERNAL(STYLE_CLASS) \
    public: \
    STYLE_CLASS##Container(); \
    virtual ~STYLE_CLASS##Container(); \
    const STYLE_CLASS* operator->() const { return static_cast<const STYLE_CLASS*>(currentStyle()); } \
    protected: \
    STYLE_CLASS##Container(class STYLE_CLASS##ContainerPrivate* dd); \
    virtual const char* styleType() const; \
    typedef const STYLE_CLASS* (STYLE_CLASS##Container::*dummyNeverToBeUsedPtr)() const; \
    dummyNeverToBeUsedPtr _dummyNeverToBeUsed(); \
    private: \
    Q_DECLARE_PRIVATE(STYLE_CLASS##Container)

// style mode macro
#define M_STYLE_MODE(MODE) \
    public: \
    void setMode##MODE();

class MWidgetController;
class MWidgetViewPrivate;
class MSceneManager;

class M_CORE_EXPORT MStyle : public QObject
{
    Q_OBJECT
    M_STYLE(MStyle)

protected:
    int references() const;
    int addReference();
    int removeReference();
    bool isOrientationDependent() const;
    void setOrientationDependent(bool orientationDependent);

    friend class MStyleSheet;
    friend class MStyleSheetPrivate;
    friend class MThemePrivate;
    friend class MStyleContainer;
    friend class MStyleSheetAttribute;
};

class M_CORE_EXPORT MStyleContainer
{
    M_STYLE_CONTAINER(MStyle)
    M_STYLE_MODE(Default)
public:
    void initialize(const QString &objectName, const QString &type, const MWidgetController *parent);

    void setObjectName(const QString &objectName);
    void setType(const QString &type);
    QString objectName() const;
    QString type() const;
    void setParent(const MWidgetController *parent);
    const MWidgetController *parent() const;

    void setSceneManager(MSceneManager *sceneManager);
    const MSceneManager *sceneManager() const;

    void updateCurrentStyle() const;

protected:
    void reloadStyles();
    const MStyle *currentStyle() const;

    void setCurrentMode(const QString &mode);
    QString currentMode() const;

    friend class MTheme;
    friend class MWidgetViewPrivate; // it needs to call reloadStyles()
};

#endif
