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
#define M_STYLE_CONTAINER(STYLE_CLASS) \
    public: \
    STYLE_CLASS##Container(); \
    virtual ~STYLE_CLASS##Container(); \
    const STYLE_CLASS* operator->() const; \
    protected: \
    STYLE_CLASS##Container(class STYLE_CLASS##ContainerPrivate* dd); \
    virtual const char* styleType() const; \
    class STYLE_CLASS##ContainerPrivate * const d_ptr; \
    private: \
    Q_DECLARE_PRIVATE(STYLE_CLASS##Container)

// style container macro for internal styles
#define M_STYLE_CONTAINER_INTERNAL(STYLE_CLASS) \
    public: \
    STYLE_CLASS##Container(); \
    virtual ~STYLE_CLASS##Container(); \
    const STYLE_CLASS* operator->() const; \
    protected: \
    STYLE_CLASS##Container(class STYLE_CLASS##ContainerPrivate* dd); \
    virtual const char* styleType() const; \
    private: \
    Q_DECLARE_PRIVATE(STYLE_CLASS##Container)

// style mode macro
#define M_STYLE_MODE(MODE) \
    public: \
    void setMode##MODE();

class MWidgetController;

class M_EXPORT MStyle : public QObject
{
    Q_OBJECT
    M_STYLE(MStyle)

protected:
    int references() const;
    int addReference();
    int removeReference();

    friend class MStyleSheet;
};

class M_EXPORT MStyleContainer
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
protected:
    void reloadStyles();
    const MStyle *currentStyle() const;

    void setCurrentMode(const QString &mode);
    QString currentMode() const;

    friend class MTheme;
};

#endif
