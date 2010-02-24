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

#include <QtGlobal>
#include <QString>
#include <QList>
#include <QDir>
#include <QFile>
#include <QFileInfo>
#include <QTextStream>
#include <QRegExp>
#include <QDebug>

class StyleAttribute
{
public:
    StyleAttribute(const QString &type,
                   const QString &name,
                   const QString &camelName,
                   bool ptr) :
        m_type(type),
        m_name(name),
        m_camelName(camelName),
        m_ptr(ptr) {
    }

    const QString &type() const {
        return m_type;
    }
    const QString &name() const {
        return m_name;
    }
    const QString &camelName() const {
        return m_camelName;
    }
    bool ptr() const {
        return m_ptr;
    }
private:
    QString m_type;
    QString m_name;
    QString m_camelName;
    bool m_ptr;
};

class StyleClass
{
public:
    StyleClass(const QString &name,
               const QString &superClassName,
               bool internal,
               const QList<StyleAttribute>& attributes) :
        m_name(name),
        m_superClassName(superClassName),
        m_internal(internal),
        m_attributes(attributes) {
    }

    const QString &name() const {
        return m_name;
    }
    const QString &superClassName() const {
        return m_superClassName;
    }
    bool internal() const {
        return m_internal;
    }
    const QList<StyleAttribute>& attributes() const {
        return m_attributes;
    }
private:
    QString m_name;
    QString m_superClassName;
    bool m_internal;
    QList<StyleAttribute> m_attributes;
};

class ModelProperty
{
public:
    ModelProperty(const QString &type,
                  const QString &name,
                  const QString &camelName,
                  bool generate,
                  const QString &defaultValue,
                  bool ptr) :
        m_type(type),
        m_name(name),
        m_camelName(camelName),
        m_generate(generate),
        m_defaultValue(defaultValue.trimmed()),
        m_ptr(ptr) {
    }

    const QString &type() const {
        return m_type;
    }
    const QString &name() const {
        return m_name;
    }
    const QString &camelName() const {
        return m_camelName;
    }
    bool generate() const {
        return m_generate;
    }
    const QString &defaultValue() const {
        return m_defaultValue;
    }
    bool ptr() const {
        return m_ptr;
    }
private:
    QString m_type;
    QString m_name;
    QString m_camelName;
    bool m_generate;
    QString m_defaultValue;
    bool m_ptr;
};

class ModelClass
{
public:
    ModelClass(const QString &name,
               const QString &superClassName,
               bool generatePrivate,
               bool internal,
               const QList<ModelProperty>& properties) :
        m_name(name),
        m_superClassName(superClassName),
        m_generatePrivate(generatePrivate),
        m_internal(internal),
        m_properties(properties) {
    }

    const QString &name() const {
        return m_name;
    }
    const QString &superClassName() const {
        return m_superClassName;
    }
    bool generatePrivate() const {
        return m_generatePrivate;
    }
    bool internal() const {
        return m_internal;
    }
    const QList<ModelProperty>& properties() const {
        return m_properties;
    }
private:
    QString m_name;
    QString m_superClassName;
    bool m_generatePrivate;
    bool m_internal;
    QList<ModelProperty> m_properties;
};


StyleClass *parseStyleClassHeader(const QString &styleHeader, QList<QString>& modes)
{
    QFile file(styleHeader);
    if (!file.open(QFile::ReadOnly))
        qFatal("Could not open header file for reading");

    QTextStream stream(&file);
    QString line = stream.readAll();

    // remove comments
    line.replace(QRegExp("\\/\\/[^\\n\\r]+"), "");
    line.replace(QRegExp("\\/\\*[\\s\\S]*?\\*\\/"), "");

    QString className;
    QString superClassName;
    bool internal = false;

    // search "class XX : public YY"
    QRegExp classRegExp("\\s*class\\s+(DUI_EXPORT\\s+)?(\\w+)\\s*:\\s*public\\s+(\\w+)\\s*");
    int index = line.indexOf(classRegExp);
    if (index == -1) {
        return NULL;
    }

    index += classRegExp.matchedLength();
    className = classRegExp.cap(2);
    superClassName = classRegExp.cap(3);

    // search "DUI_STYLE(XX)" or "DUI_STYLE_INTERNAL(XX)"
    QRegExp styleRegExp("\\s*DUI_STYLE\\s*\\(\\s*\\w+\\s*\\)\\s*");
    QRegExp styleInternalRegExp("\\s*DUI_STYLE_INTERNAL\\s*\\(\\s*\\w+\\s*\\)\\s*");

    int start = index;
    if ((index = line.indexOf(styleRegExp, start)) != -1) {
        index += styleRegExp.matchedLength();
        internal = false;
    } else if ((index = line.indexOf(styleInternalRegExp, start)) != -1) {
        index += styleInternalRegExp.matchedLength();
        internal = true;
    } else {
        return NULL;
    }

    QList<StyleAttribute> attributes;
    // search all "DUI_STYLE_ATTRIBUTE(A,B,C)"
    QRegExp attributeRegExp("\\s*DUI_STYLE_ATTRIBUTE\\s*\\(\\s*(\\w+\\:*\\w*)\\s*,\\s*(\\w+)\\s*,\\s*(\\w+)\\s*\\)\\s*");
    QRegExp attributePtrRegExp("\\s*DUI_STYLE_PTR_ATTRIBUTE\\s*\\(\\s*(\\w+\\:*\\w*\\s*\\*+)\\s*,\\s*(\\w+)\\s*,\\s*(\\w+)\\s*\\)\\s*");
    QRegExp endRegExp("\\s*\\};\\s*");

    int result3 = line.indexOf(endRegExp, index);
    if (result3 == -1) {
        return NULL;
    }

    while (true) {
        int result = line.indexOf(attributeRegExp, index);
        int result2 = line.indexOf(attributePtrRegExp, index);

        if (result == -1 && result2 == -1) {
            index = result3 + endRegExp.matchedLength();
            break;
        }

        // check which one is closer
        if (result != -1 && result2 != -1) {
            if (result2 < result) {
                // make sure we don't pass the end of the block
                if (result3 < result2) {
                    index = result3 + endRegExp.matchedLength();
                    break;
                }
                result = -1;
            } else {
                // make sure we don't pass the end of the block
                if (result3 < result) {
                    index = result3 + endRegExp.matchedLength();
                    break;
                }
                result2 = -1;
            }
        }

        if (result != -1) {
            attributes.append(StyleAttribute(attributeRegExp.cap(1),
                                             attributeRegExp.cap(2),
                                             attributeRegExp.cap(3),
                                             false));
            index = result + attributeRegExp.matchedLength();
        } else {
            attributes.append(StyleAttribute(attributePtrRegExp.cap(1),
                                             attributePtrRegExp.cap(2),
                                             attributePtrRegExp.cap(3),
                                             true));
            index = result2 + attributePtrRegExp.matchedLength();
        }
    }

    QRegExp modeRegExp("\\s*DUI_STYLE_MODE\\s*\\(\\s*(\\w+)\\s*\\)\\s*");
    while (true) {
        int result = line.indexOf(modeRegExp, index);
        if (result == -1)
            break;

        modes.append(modeRegExp.cap(1));
        index = result + modeRegExp.matchedLength();
    }

    return new StyleClass(className, superClassName, internal, attributes);
}

ModelClass *parseModelClassHeader(const QString &modelHeader)
{
    QFile file(modelHeader);
    if (!file.open(QFile::ReadOnly))
        qFatal("Could not open header file for reading");

    QTextStream stream(&file);
    QString line = stream.readAll();

    // remove comments
    line.replace(QRegExp("\\/\\/[^\\n\\r]+"), "");
    line.replace(QRegExp("\\/\\*[\\s\\S]*?\\*\\/"), "");

    QString className;
    QString superClassName;
    bool generatePrivate = false;
    bool internal = false;

    const QString spaces("\\s*");
    const QString comma(",");
    const QString colon(":");
    const QString parenO("\\(");
    const QString parenC("\\)");
    const QString emptyParen("\\(\\)");
    const QString angleO("<");
    const QString angleC(">");
    const QString braceO("\\{");
    const QString nameSpace("(?:\\w+::)");
    const QString typeName("\\w+");
    const QString pointer("\\*");
    const QString templateName("\\w+");
    const QString plainParam("(\\w+)");
    const QString boolParam("(true|false)");
    const QString anyParam("([^\\n\\r]+)"); //("(.+)");

    // search "class XX : public YY"
    QRegExp classRegExp(
        spaces +
        "class" +
        spaces +
        "(?:DUI_EXPORT)" + "{,1}" +
        spaces +
        '(' +
        typeName +
        ')' +
        spaces +
        colon +
        spaces +
        "public" +
        spaces +
        '(' +
        typeName +
        ')' +
        spaces +
        braceO + '*'
    );

    int index = line.indexOf(classRegExp);
    if (index == -1) {
        return NULL;
    }

    index += classRegExp.matchedLength();
    className = classRegExp.cap(1);
    superClassName = classRegExp.cap(2);

    // search "DUI_MODEL(XX)" or "DUI_MODEL_INTERNAL(XX)" or
    // "DUI_MODEL_WITH_PRIVATE(XX)" or "DUI_MODEL_INTERNAL_WITH_PRIVATE(XX)"
    QRegExp modelRegExp(
        spaces + "DUI_MODEL" + spaces + parenO + spaces + typeName + spaces + parenC + spaces
    );
    QRegExp modelInternal(
        spaces + "DUI_MODEL_INTERNAL" + spaces + parenO + spaces + typeName + spaces + parenC + spaces
    );
    QRegExp modelWithPrivateRegExp(
        spaces + "DUI_MODEL_WITH_PRIVATE" + spaces + parenO + spaces + typeName + spaces + parenC + spaces
    );
    QRegExp modelInternalWithPrivateRegExp(
        spaces + "DUI_MODEL_INTERNAL_WITH_PRIVATE" + spaces + parenO + spaces + typeName + spaces + parenC + spaces
    );

    int start = index;
    if ((index = line.indexOf(modelRegExp, start)) != -1) {
        index += modelRegExp.matchedLength();
        generatePrivate = false;
        internal = false;
    } else if ((index = line.indexOf(modelInternal, start)) != -1) {
        index += modelInternal.matchedLength();
        generatePrivate = false;
        internal = true;
    } else if ((index = line.indexOf(modelWithPrivateRegExp, start)) != -1) {
        index += modelWithPrivateRegExp.matchedLength();
        generatePrivate = true;
        internal = false;
    } else if ((index = line.indexOf(modelInternalWithPrivateRegExp, start)) != -1) {
        index += modelInternalWithPrivateRegExp.matchedLength();
        generatePrivate = true;
        internal = true;
    } else {
        return NULL;
    }

    QList<ModelProperty> properties;
    // search all " DUI_MODEL_PROPERTY(A,B,C,D,E)"

    QRegExp propertyRegExp(
        spaces +
        "DUI_MODEL_PROPERTY" +
        spaces + parenO + spaces +
        '(' +
        "(?:" +
        nameSpace + "{,1}" +
        typeName +
        spaces +
        pointer + "{,1}" +
        ')' +
        '|' +
        "(?:" +
        templateName +
        angleO +
        spaces +
        typeName +
        spaces +
        pointer + "{,1}" +
        spaces +
        angleC +
        ')' +
        ')' +
        spaces + comma + spaces +
        plainParam +
        spaces + comma + spaces +
        plainParam +
        spaces + comma + spaces +
        plainParam +
        spaces + comma + spaces +
        anyParam +
        spaces + parenC + spaces
    );

    QRegExp propertyPtrRegExp(
        spaces +
        "DUI_MODEL_PTR_PROPERTY" +
        spaces + parenO + spaces +
        '(' +
        "(?:" +
        nameSpace + "{,1}" +
        typeName +
        spaces +
        pointer + "{,1}" +
        spaces +
        ')' +
        '|' +
        "(?:" +
        templateName +
        angleO +
        spaces +
        typeName +
        spaces +
        pointer + "{,1}" +
        spaces +
        angleC +
        ')' +
        ')' +
        spaces + comma + spaces +
        plainParam +
        spaces + comma + spaces +
        plainParam +
        spaces + comma + spaces +
        boolParam +
        spaces + comma + spaces +
        anyParam +
        spaces + parenC + spaces
    );

    while (true) {
        int result = line.indexOf(propertyRegExp, index);
        int result2 = line.indexOf(propertyPtrRegExp, index);

        if (result == -1 && result2 == -1)
            break;

        if (result != -1 && result2 != -1) {
            if (result2 < result) {
                result = -1;
            } else {
                result2 = -1;
            }
        }

        if (result != -1) {
            properties.append(ModelProperty(propertyRegExp.cap(1),
                                            propertyRegExp.cap(2),
                                            propertyRegExp.cap(3),
                                            propertyRegExp.cap(4) == QString("true"),
                                            propertyRegExp.cap(5),
                                            false));
            index = result + propertyRegExp.matchedLength();
        } else {
            properties.append(ModelProperty(propertyPtrRegExp.cap(1),
                                            propertyPtrRegExp.cap(2),
                                            propertyPtrRegExp.cap(3),
                                            propertyPtrRegExp.cap(4) == QString("true"),
                                            propertyPtrRegExp.cap(5),
                                            true));
            index = result2 + propertyPtrRegExp.matchedLength();
        }
    }

    return new ModelClass(className, superClassName, generatePrivate, internal, properties);
}

//////////////////////////
// generate styledata.h //
//////////////////////////
void generateStyleDataHeader(const StyleClass *c, const QList<QString>& modes, const QString headerfilename)
{
    // #ifndef DUILABELSTYLEDATA_H
    // #define DUILABELSTYLEDATA_H
    //
    // ---- if(internal) ----
    // ---- if(QFile::exists("duiwidgetstyledata.h")) ----
    // #include "duiwidgetstyledata.h"
    // ---- else                                      ----
    // #include "gen_duiwidgetstyledata.h"
    //
    // ---- endif                                     ----
    // ---- endif        ----
    // #include "duilabelstyle.h"
    // ---- if(internal) ----
    // class DuiLabelStyleData : public DuiWidgetStyleData
    // ---- else         ----
    // class DuiLabelStyleData
    // ---- endif        ----
    // {
    //     friend class DuiLabelStyle;
    // public:
    //     DuiLabelStyleData();
    //     virtual ~DuiLabelStyleData() {}
    // private:
    //     QFont font;
    //     QColor color;
    // };
    //
    // ---- if(internal) ----
    // class DuiLabelStyleContainerPrivate : DuiWidgetStyleContainer
    // ---- else         ----
    // class DuiLabelStyleContainerPrivate
    // ---- endif        ----
    // {
    //     friend class DuiLabelContainer;
    // public:
    //     DuiLabelContainerPrivate();
    //     virtual ~DuiStyleContainerPrivate();
    // private:
    //     const DuiStyle* activeStyle[2];
    //     const DuiStyle* disabledStyle[2];
    // };
    //
    // #endif
    //

    QFile file(headerfilename);
    if (!file.open(QFile::WriteOnly))
        qFatal("Could not open data header for writing");

    QFileInfo info(headerfilename);

    QTextStream h(&file);

    h << "#ifndef " << c->name().toUpper() << "DATA_H\n";
    h << "#define " << c->name().toUpper() << "DATA_H\n\n";
    if (c->internal()) {
        if (QFile::exists(info.absolutePath() + QDir::separator() + c->superClassName().toLower() + "data.h"))
            h << "#include \"" << c->superClassName().toLower() << "data.h\"\n";
        else
            h << "#include \"gen_" << c->superClassName().toLower() << "data.h\"\n";
    }
    h << "#include \"" << c->name().toLower() << ".h\"\n\n";
    if (c->superClassName() == QString("DuiStyle") && c->internal())
        h << "#include \"" << c->superClassName().toLower() << "_p.h\"\n\n";

    if (c->internal())
        h << "class " << c->name() << "Data : public " << c->superClassName() << "Data\n";
    else
        h << "class " << c->name() << "Data\n";

    h << "{\n";
    h << "    friend class " << c->name() << ";\n";
    h << "public:\n";
    h << "    " << c->name() << "Data();\n";
    h << "    virtual ~" << c->name() << "Data() {}\n";
    h << "private:\n";

    if (c->name() == QString("DuiStyle")) {
        h << "int references;";
    }

    QList<StyleAttribute>::const_iterator i = c->attributes().constBegin();
    for (; i != c->attributes().constEnd(); ++i) {
        if (i->ptr())
            h << "    const " << i->type() << " " << i->name() << ";\n";
        else
            h << "    " << i->type() << " " << i->name() << ";\n";
    }

    h << "};\n\n";

    if (c->name() != QString("DuiStyle")) {
        // container private
        if (c->internal())
            h << "class " << c->name() << "ContainerPrivate : public " << c->superClassName() << "ContainerPrivate\n";
        else
            h << "class " << c->name() << "ContainerPrivate\n";

        h << "{\n";
        h << "    friend class " << c->name() << "Container;\n";
        h << "public:\n";
        h << "    " << c->name() << "ContainerPrivate();\n";
        h << "    virtual ~" << c->name() << "ContainerPrivate();\n";
        h << "private:\n";
        QList<QString>::const_iterator iterator = modes.constBegin();
        for (; iterator != modes.constEnd(); ++iterator) {
            h << "    const DuiStyle* " << iterator->toLower() << "Style[2];\n";
        }

        h << "};\n\n";
    }
    h << "#endif\n";
}

////////////////////////////
// generate styledata.cpp //
////////////////////////////
void generateStyleDataCpp(const StyleClass *c, const QList<QString>& modes, const QString &cppfilename)
{
    // #include "duilabelstyle.h"
    // #include "gen_duilabelstyledata.h";
    // #include "duistylecreator.h"
    //
    // DUI_REGISTER_STYLE(DuiLabelStyle)
    //
    // DuiLabelStyleData::DuiLabelStyleData()
    // {
    // ---- all pointer attributes set to 0.
    // }
    //
    // DuiLabelStyle::DuiLabelStyle() :
    // ---- if(internal) ----
    //     DuiWidgetStyle(new DuiButtonModelData)
    // ---- else         ----
    //     DuiWidgetStyle(),
    //     data(new DuiLabelStyleData)
    // ---- endif        ----
    // {
    // }
    //
    // DuiLabelStyle::~DuiLabelStyle()
    // {
    // ---- if(!internal) ----
    //     delete data;
    // ---- endif         ----
    // }
    //
    // DuiLabelStyle::DuiLabelStyle(DuiLabelStyleData* data, DuiButtonModelPrivate* dd) :
    // ---- if(internal) ----
    //     DuiWidgetModel(data, dd == NULL ? new DuiButtonModelPrivate : dd)
    // ---- else         ----
    //     DuiWidgetModel(),
    //     data(data),
    //     d_ptr(dd == NULL ? new DuiButtonModelData : dd)
    // ---- endif        ----
    // {
    // }
    //
    // const QFont& DuiLabelStyle::font() const
    // {
    //     return static_cast<const DuiLabelStyleData*>(data)->font;
    // }
    //
    // void DuiButtonModel::setText(const QFont& font)
    // {
    //     if(static_cast<DuiLabelStyleData*>(data)->font != font) {
    //         static_cast<DuiLabelStyleData*>(data)->font = font;
    //         memberModified(Text);
    //     }
    // }
    //
    // ///////////////
    // // CONTAINER //
    // ///////////////
    //
    // DuiLabelStyleContainerPrivate::DuiLabelStyleContainerPrivate() :
    //     activeStyle(0),
    //     disabledStyle(0)
    // {
    //     activeStyle[Dui::Landscape] = NULL;
    //     activeStyle[Dui::Portrait] = NULL;
    //     disabledStyle[Dui::Landscape] = NULL;
    //     disabledStyle[Dui::Portrait] = NULL;
    // }
    //
    // DuiStyleContainerPrivate::~DuiStyleContainerPrivate()
    // {
    //     DuiTheme::releaseStyle(activeStyle[Dui::Landscape]);
    //     DuiTheme::releaseStyle(activeStyle[Dui::Portrait]);
    //     DuiTheme::releaseStyle(disabledStyle[Dui::Landscape]);
    //     DuiTheme::releaseStyle(disabledStyle[Dui::Portrait]);
    // }
    //
    // DuiLabelStyleContainer::DuiLabelStyleContainer() :
    // ---- if(internal) ----
    //     DuiStyleContainer(new DuiLabelStyleContainerPrivate)
    // ---- else         ----
    //     DuiStyleContainer(),
    //     d_ptr(new DuiLabelStyleContainerPrivate)
    // ---- endif        ----
    // {
    // }
    //
    // DuiLabelStyleContainer::DuiLabelStyleContainer(DuiLabelStyleContainerPrivate* dd) :
    // ---- if(internal) ----
    //     DuiStyleContainer(dd)
    // ---- else         ----
    //     DuiStyleContainer(),
    //     d_ptr(dd)
    // ---- endif        ----
    // {
    // }
    //
    // DuiLabelStyleContainer::~DuiLabelStyleContainer()
    // {
    // ---- if(!internal) ----
    //     delete d_ptr;
    // ---- endif         ----
    // }
    //
    // const DuiLabelStyle* DuiLabelStyleContainer::operator->() const
    // {
    //     return static_cast<const DuiLabelStyle*>(currentStyle());
    // }
    //
    // void DuiLabelStyleContainer::reloadStyles()
    // {
    //     DuiWidgetStyleContainer::reloadStyles();
    //     Q_D(DuiLabelStyleContainer);
    //     const DuiStyle* tmp;
    //     tmp = d->activeStyle[Dui::Landscape];
    //     d->activeStyle[Dui::Landscape] = DuiTheme::style(styleType(), objectName(), "active", Dui::Landscape, parent());
    //     DuiTheme::releaseStyle(tmp);
    //     tmp = d->activeStyle[Dui::Portrait];
    //     d->activeStyle[Dui::Portrait] = DuiTheme::style(styleType(), objectName(), "active", Dui::Portrait, parent());
    //     DuiTheme::releaseStyle(tmp);
    //
    //     tmp = d->disabledStyle[Dui::Landscape];
    //     d->disabledStyle[Dui::Landscape] = DuiTheme::style(styleType(), objectName(), "disabled", Dui::Landscape, parent());
    //     DuiTheme::releaseStyle(tmp);
    //     tmp = d->disabledStyle[Dui::Portrait];
    //     d->disabledStyle[Dui::Portrait] = DuiTheme::style(styleType(), objectName(), "disabled", Dui::Portrait, parent());
    //     DuiTheme::releaseStyle(tmp);
    // }
    //
    // const char* DuiLabelStyleContainer::styleType() const
    // {
    //     return "DuiLabelStyle";
    // }
    //
    // void DuiLabelStyleContainer::setModeActive()
    // {
    //     Q_D(DuiLabelStyleContainer);
    //     setCurrentStyle(d->activeStyle[Dui::Landscape], d->activeStyle[Dui::Portrait]);
    // }
    //
    // void DuiLabelStyleContainer::setModeDisabled()
    // {
    //     Q_D(DuiLabelStyleContainer);
    //     setCurrentStyle(d->disabledStyle[Dui::Landscape], d->disabledStyle[Dui::Portrait]);
    // }

    QFile file(cppfilename);
    if (!file.open(QFile::WriteOnly))
        qFatal("Could not open data cpp for writing");

    QTextStream cpp(&file);


    cpp << "#include \"" << c->name().toLower() << ".h\"\n";
    cpp << "#include \"gen_" << c->name().toLower() << "data.h\"\n";
    cpp << "#include \"duistylecreator.h\"\n";
    cpp << "#include \"duitheme.h\"\n\n";

    cpp << "DUI_REGISTER_STYLE(" << c->name() << ")\n\n";

    // public constructor of Data class.
    cpp << c->name() << "Data::" << c->name() << "Data()\n";

    bool initializerListStarted = false;
    if (c->name() == QString("DuiStyle")) {
        cpp << " : references(1)";
        initializerListStarted = true;
    }

    QList<StyleAttribute>::const_iterator i = c->attributes().constBegin();
    for (; i != c->attributes().constEnd(); ++i) {

        if (!initializerListStarted) {
            cpp << " : ";
            initializerListStarted = true;
        } else {
            cpp << ",\n   ";
        }

        if (i->ptr())
            cpp << " " << i->name() << "(0)";
        else
            cpp << " " << i->name() << "()";
    }

    if (initializerListStarted)
        cpp << "\n";

    cpp << "{\n";
    cpp << "}\n\n";

    // public constructor
    cpp << c->name() << "::" << c->name() << "() :\n";
    if (c->internal()) {
        cpp << "    " << c->superClassName() << "(new " << c->name() << "Data)\n";
    } else {
        // not internal
        cpp << "    " << c->superClassName() << "(),\n";
        cpp << "    data(new " << c->name() << "Data)\n";
    }
    cpp << "{\n";
    cpp << "}\n\n";

    // destructor
    cpp << c->name() << "::~" << c->name() << "()\n";
    cpp << "{\n";
    if (!c->internal()) {
        cpp << "    delete data;\n";
    }
    cpp << "}\n\n";

    // protected costructor
    cpp << c->name() << "::" << c->name() << "(" << c->name() << "Data* data) :\n";
    if (c->internal()) {
        cpp << "    " << c->superClassName() << "(data)\n";
    } else {
        // not internal
        cpp << "    " << c->superClassName() << "(),\n";
        cpp << "    " << "data(data)\n";
    }
    cpp << "{\n";
    cpp << "}\n\n";

    // attributes
    i = c->attributes().begin();
    for (; i != c->attributes().end(); ++i) {
        // getter
        if (i->ptr())
            cpp << "const " << i->type() << " " << c->name() << "::" << i->name() << "() const\n";
        else
            cpp << "const " << i->type() << "& " << c->name() << "::" << i->name() << "() const\n";
        cpp << "{\n";
        cpp << "    return static_cast<const " << c->name() << "Data*>(data)->" << i->name() << ";\n";
        cpp << "}\n\n";

        // setter
        if (i->ptr())
            cpp << "void " << c->name() << "::set" << i->camelName() << "(const " << i->type() << " " << i->name() << ")\n";
        else
            cpp << "void " << c->name() << "::set" << i->camelName() << "(const " << i->type() << "& " << i->name() << ")\n";
        cpp << "{\n";
        cpp << "    static_cast<" << c->name() << "Data*>(data)->" << i->name() << " = " << i->name() << ";\n";
        cpp << "}\n\n";
    }

    if (c->name() != QString("DuiStyle")) {
        cpp << "///////////////\n";
        cpp << "// CONTAINER //\n";
        cpp << "///////////////\n\n";

        // private class constructor
        cpp << c->name() << "ContainerPrivate::" << c->name() << "ContainerPrivate()\n";

        cpp << "{\n";
        QList<QString>::const_iterator iterator = modes.constBegin();
        for (; iterator != modes.constEnd(); ++iterator) {
            cpp << "    " << iterator->toLower() << "Style[Dui::Landscape] = NULL;\n";
            cpp << "    " << iterator->toLower() << "Style[Dui::Portrait] = NULL;\n";
        }
        cpp << "}\n\n";

        // private class destructor
        cpp << c->name() << "ContainerPrivate::~" << c->name() << "ContainerPrivate()\n";
        cpp << "{\n";
        iterator = modes.begin();
        for (; iterator != modes.end(); ++iterator) {
            cpp << "    DuiTheme::releaseStyle(" << iterator->toLower() << "Style[Dui::Landscape]);\n";
            cpp << "    DuiTheme::releaseStyle(" << iterator->toLower() << "Style[Dui::Portrait]);\n";
        }
        cpp << "}\n\n\n";


        // public constructor
        cpp << c->name() << "Container::" << c->name() << "Container() :\n";
        if (c->internal()) {
            cpp << "    " << c->superClassName() << "Container(new " << c->name() << "ContainerPrivate)\n";
        } else {
            cpp << "    " << c->superClassName() << "Container(),\n";
            cpp << "    d_ptr(new " << c->name() << "ContainerPrivate)\n";
        }
        cpp << "{\n";
        cpp << "}\n\n";

        // protected constructor
        cpp << c->name() << "Container::" << c->name() << "Container(" << c->name() << "ContainerPrivate* dd) :\n";
        if (c->internal()) {
            cpp << "    " << c->superClassName() << "Container(dd)\n";
        } else {
            cpp << "    " << c->superClassName() << "Container(),\n";
            cpp << "    d_ptr(dd)\n";
        }
        cpp << "{\n";
        cpp << "}\n\n";

        // destructor
        cpp << c->name() << "Container::~" << c->name() << "Container()\n";
        cpp << "{\n";
        if (!c->internal())
            cpp << "    delete d_ptr;\n";
        cpp << "}\n\n";

        // -> operator
        cpp << "const " << c->name() << "* " << c->name() << "Container::operator->() const\n";
        cpp << "{\n";
        cpp << "    return static_cast<const " << c->name() << "*>(currentStyle());\n";
        cpp << "}\n\n";

        // reloadStyles
        // TODO: remove this once we are allowed again to break API
        cpp << "void " << c->name() << "Container::reloadStyles()\n";
        cpp << "{\n";
        cpp << "    " << "DuiStyleContainer::reloadStyles();\n";
        cpp << "}\n\n";

        // styleType
        cpp << "const char* " << c->name() << "Container::styleType() const\n";
        cpp << "{\n";
        cpp << "    return \"" << c->name() << "\";\n";
        cpp << "}\n\n";

        iterator = modes.begin();
        for (; iterator != modes.end(); ++iterator) {
            cpp << "void " << c->name() << "Container::setMode" << *iterator << "()\n";
            cpp << "{\n";
            cpp << "    setCurrentMode(\"" << iterator->toLower() << "\");\n";
            cpp << "}\n\n";
        }
    }
}

//////////////////////////
// generate modeldata.h //
//////////////////////////
void generateModelDataHeader(const ModelClass *c, const QString &headerfilename)
{
    // #ifndef DUIBUTTONMODELDATA_H
    // #define DUIBUTTONMODELDATA_H
    //
    // ---- if(internal) ----
    // ---- if(QFile::exists("duiwidgetmodeldata.h")) ----
    // #include "duiwidgetmodeldata.h"
    // ---- else                                      ----
    // #include "gen_duiwidgetmodeldata.h"
    // ---- endif                                     ----
    // ---- endif        ----
    // #include "duibuttonmodel.h"
    //
    // ---- if(internal) ----
    // class DuiButtonModelData : public DuiWidgetModelData
    // ---- else         ----
    // class DuiButtonModelData
    // ---- endif        ----
    // {
    //     friend class DuiButtonModel;
    // public:
    //     virtual ~DuiButtonModelData() {}
    // private:
    //     QString text;
    //     QString iconID;
    // };
    // #endif
    //

    QFile file(headerfilename);
    if (!file.open(QFile::WriteOnly))
        qFatal("Could not open data header for writing");

    QFileInfo info(headerfilename);

    QTextStream h(&file);

    h << "#ifndef " << c->name().toUpper() << "DATA_H\n";
    h << "#define " << c->name().toUpper() << "DATA_H\n\n";
    if (c->internal()) {
        if (QFile::exists(info.absolutePath() + QDir::separator() + c->superClassName().toLower() + "data.h"))
            h << "#include \"" << c->superClassName().toLower() << "data.h\"\n";
        else
            h << "#include \"gen_" << c->superClassName().toLower() << "data.h\"\n";
    }
    h << "#include \"" << c->name().toLower() << ".h\"\n\n";

    if (c->internal())
        h << "class " << c->name() << "Data : public " << c->superClassName() << "Data\n";
    else
        h << "class " << c->name() << "Data\n";

    h << "{\n";
    h << "    friend class " << c->name() << ";\n";
    h << "public:\n";
    h << "    virtual ~" << c->name() << "Data() {}\n";
    h << "private:\n";

    QList<ModelProperty>::const_iterator i = c->properties().constBegin();
    for (; i != c->properties().constEnd(); ++i) {
        if (i->ptr()) {
            if (i->generate())
                h << "    " << i->type() << " " << i->name() << ";\n";
        } else {
            h << "    " << i->type() << " " << i->name() << ";\n";
        }
    }

    h << "};\n";
    h << "#endif\n";
}


////////////////////////////
// generate modeldata.cpp //
////////////////////////////
void generateModelDataCpp(const ModelClass *c, const QString &cppfilename)
{
    // #include "duibuttonmodel.h"
    // ---- if(generatePrivate) ----
    // #include "duibuttonmodel_p.h"
    // ---- endif               ----
    // #include "gen_duibuttonmodeldata.h";
    //
    // const char* const DuiButtonModel::Text = "text";
    // const char* const DuiButtonModel::IconID = "iconID";
    //
    // DuiButtonModel::DuiButtonModel() :
    // ---- if(generatePrivate && internal) ----
    //     DuiWidgetModel(new DuiButtonModelData, new DuiButtonModelPrivate)
    // ---- else if(generatePrivate)        ----
    //     DuiWidgetModel(),
    //     data(new DuiButtonModelData),
    //     d_ptr(new DuiButtonModelPrivate)
    // ---- else if(internal)               ----
    //     DuiWidgetModel(new DuiButtonModelData)
    // ---- else                            ----
    //     DuiWidgetModel(),
    //     data(new DuiButtonModelData)
    // ---- endif                           ----
    // {
    //     static_cast<DuiButtonModelData*>(data)->text = "DEFAULT_A";
    //     static_cast<DuiButtonModelData*>(data)->iconID = "DEFAULT_B";
    // }
    //
    // DuiButtonModel::~DuiButtonModel()
    // {
    // ---- if(!internal)                    ----
    //     delete data;
    // ---- if(generatePrivate && !internal) ----
    //     delete d_ptr;
    // ---- endif                            ----
    // }
    //
    // ---- if(generatePrivate) ----
    // DuiButtonModel::DuiButtonModel(DuiButtonModelData* data, DuiButtonModelPrivate* dd) :
    // ---- if(internal) ----
    //     DuiWidgetModel(data, dd == NULL ? new DuiButtonModelPrivate : dd)
    // ---- else         ----
    //     DuiWidgetModel(),
    //     data(data),
    //     d_ptr(dd == NULL ? new DuiButtonModelData : dd)
    // ---- endif        ----
    // {
    //     static_cast<DuiButtonModelData*>(data)->text = "DEFAULT_A";
    //     static_cast<DuiButtonModelData*>(data)->iconID = "DEFAULT_B";
    // }
    // ---- else                ----
    // DuiButtonModel::DuiButtonModel(DuiButtonModelData* data) :
    // ---- if(internal) ----
    //     DuiWidgetModel(data)
    // ---- else         ----
    //     DuiWidgetModel(),
    //     data(data)
    // {
    // }
    // ---- endif               ----
    //
    // QString& DuiButtonModel::_text()
    // {
    //     return static_cast<DuiButtonModelData*>(data)->text;
    // }
    //
    // const QString& DuiButtonModel::_text() const
    // {
    //     return static_cast<const DuiButtonModelData*>(data)->text;
    // }
    //
    // const QString& DuiButtonModel::text() const
    // {
    //     return static_cast<const DuiButtonModelData*>(data)->text;
    // }
    //
    // void DuiButtonModel::setText(const QString& text)
    // {
    //     if(static_cast<DuiButtonModelData*>(data)->text != text) {
    //         static_cast<DuiButtonModelData*>(data)->text = text;
    //         memberModified(Text);
    //     }
    // }

    QFile file(cppfilename);
    if (!file.open(QFile::WriteOnly))
        qFatal("Could not open data cpp for writing");

    QTextStream cpp(&file);


    cpp << "#include \"" << c->name().toLower() << ".h\"\n";
    if (c->generatePrivate())
        cpp << "#include \"" << c->name().toLower() << "_p.h\"\n";
    cpp << "#include \"gen_" << c->name().toLower() << "data.h\"\n\n";

    // properties
    QList<ModelProperty>::const_iterator i = c->properties().constBegin();
    for (; i != c->properties().constEnd(); ++i) {
        cpp << "const char* const " << c->name() << "::" << i->camelName() << " = \"" << i->name() << "\";\n";
    }
    cpp << "\n";

    // public constructor
    cpp << c->name() << "::" << c->name() << "() :\n";
    if (c->internal()) {
        cpp << "    " << c->superClassName() << "(new " << c->name() << "Data";
        if (c->generatePrivate()) {
            cpp << ", new " << c->name() << "Private";
        }
        cpp << ")\n";
    } else {
        // not internal
        cpp << "    " << c->superClassName() << "(),\n";
        cpp << "    data(new " << c->name() << "Data)";
        if (c->generatePrivate()) {
            cpp << ",\n";
            cpp << "    d_ptr(new " << c->name() << "Private)";
        }
        cpp << "\n";
    }
    cpp << "{\n";
    i = c->properties().begin();
    for (; i != c->properties().end(); ++i) {

        if (i->generate() || !i->ptr()) {

            // Set the specified default value in the constructor.
            // But avoid an unnecessary and inefficient =QString().
            // It would be simpler, and more generically useful, to make the
            // 5th DUI_MODEL_PROPERTY() parameter optional, but that is difficult
            // because it is also a C macro.
            const QString defaultValue = i->defaultValue();
            if (!defaultValue.isEmpty() && (defaultValue != "QString()") && (defaultValue != "QString::null")) {
                cpp << "    static_cast<" << c->name() << "Data*>(data)->" << i->name() << " = " << defaultValue << ";\n";
            }
        }
    }
    cpp << "}\n\n";

    // destructor
    cpp << c->name() << "::~" << c->name() << "()\n";
    cpp << "{\n";
    if (!c->internal()) {
        cpp << "    delete data;\n";
        if (c->generatePrivate()) {
            cpp << "    delete d_ptr;\n";
        }
    }
    cpp << "}\n\n";

    // protected constructor
    cpp << c->name() << "::" << c->name() << "(" << c->name() << "Data* data";
    if (c->generatePrivate()) {
        cpp << ", " << c->name() << "Private* dd";
    }
    cpp << ") :\n";

    if (c->internal()) {
        cpp << "    " << c->superClassName() << "(data";
        if (c->generatePrivate()) {
            cpp << ", dd == NULL ? new " << c->name() << "Private : dd";
        }
        cpp << ")\n";
    } else {
        // not internal
        cpp << "    " << c->superClassName() << "(),\n";
        cpp << "    " << "data(data)";
        if (c->generatePrivate()) {
            cpp << ",\n";
            cpp << "    d_ptr(dd == NULL ? new " << c->name() << "Private : dd)";
        }
        cpp << "\n";
    }
    cpp << "{\n";
    i = c->properties().begin();
    for (; i != c->properties().end(); ++i) {
        if (i->generate() || !i->ptr()) {

            // Set the specified default value in the constructor.
            // But avoid an unnecessary and inefficient =QString().
            // It would be simpler, and more generically useful, to make the
            // 5th DUI_MODEL_PROPERTY() parameter optional, but that is difficult
            // because it is also a C macro.
            const QString defaultValue = i->defaultValue();
            if (!defaultValue.isEmpty() && (defaultValue != "QString()") && (defaultValue != "QString::null")) {
                cpp << "    static_cast<" << c->name() << "Data*>(data)->" << i->name() << " = " << defaultValue << ";\n";
            }
        }
    }
    cpp << "}\n\n";

    // properties
    i = c->properties().begin();
    for (; i != c->properties().end(); ++i) {

        // private getter
        if (!i->ptr()) {
            cpp << i->type() << "& " << c->name() << "::_" << i->name() << "()\n";
            cpp << "{\n";
            cpp << "    return static_cast<" << c->name() << "Data*>(data)->" << i->name() << ";\n";
            cpp << "}\n\n";
        }

        // private const getter
        if (!i->ptr()) {
            cpp << "const " << i->type() << "& " << c->name() << "::_" << i->name() << "() const\n";
            cpp << "{\n";
            cpp << "    return static_cast<const " << c->name() << "Data*>(data)->" << i->name() << ";\n";
            cpp << "}\n\n";
        }

        if (i->generate()) {
            // getter
            if (i->ptr())
                cpp << i->type() << " " << c->name() << "::" << i->name() << "() const\n";
            else
                cpp << "const " << i->type() << "& " << c->name() << "::" << i->name() << "() const\n";
            cpp << "{\n";
            cpp << "    return static_cast<const " << c->name() << "Data*>(data)->" << i->name() << ";\n";
            cpp << "}\n\n";

            // setter
            if (i->ptr())
                cpp << "void " << c->name() << "::set" << i->camelName() << "(" << i->type() << " " << i->name() << ")\n";
            else
                cpp << "void " << c->name() << "::set" << i->camelName() << "(const " << i->type() << "& " << i->name() << ")\n";
            cpp << "{\n";
            cpp << "    if(static_cast<" << c->name() << "Data*>(data)->" << i->name() << " != " << i->name() << ") {\n";
            cpp << "        static_cast<" << c->name() << "Data*>(data)->" << i->name() << " = " << i->name() << ";\n";
            cpp << "        memberModified(" << i->camelName() << ");\n";
            cpp << "    }\n";
            cpp << "}\n\n";
        }
    }
}

void parseAndGenerateStyleData(QString styleHeader, QString outdir)
{
    QList<QString> modes;
    StyleClass *c = parseStyleClassHeader(styleHeader, modes);
    if (!c) {
        qWarning() << "Could not parse" << styleHeader;
        return;
    }

    QFileInfo info(styleHeader);
    QString styleDataHeader;
    QString styleDataCpp;
    if (outdir.isEmpty()) {
        styleDataHeader = "gen_" + c->name().toLower() + "data.h";
        styleDataCpp = "gen_" + c->name().toLower() + "data.cpp";
    } else {
        if (outdir[outdir.count()-1] != QDir::separator())
            outdir += QDir::separator();
        styleDataHeader = outdir + "gen_" + c->name().toLower() + "data.h";
        styleDataCpp = outdir + "gen_" + c->name().toLower() + "data.cpp";
    }

    generateStyleDataHeader(c, modes, styleDataHeader);
    generateStyleDataCpp(c, modes, styleDataCpp);
    delete c;
}

void parseAndGenerateModelData(QString modelHeader, QString outdir)
{
    ModelClass *c = parseModelClassHeader(modelHeader);
    if (!c) {
        qWarning() << "Could not parse" << modelHeader;
        return;
    }

    QFileInfo info(modelHeader);
    QString modelDataHeader;
    QString modelDataCpp;
    if (outdir.isEmpty()) {
        modelDataHeader = "gen_" + c->name().toLower() + "data.h";
        modelDataCpp = "gen_" + c->name().toLower() + "data.cpp";
    } else {
        if (outdir[outdir.count()-1] != QDir::separator())
            outdir += QDir::separator();
        modelDataHeader = outdir + "gen_" + c->name().toLower() + "data.h";
        modelDataCpp = outdir + "gen_" + c->name().toLower() + "data.cpp";
    }

    generateModelDataHeader(c, modelDataHeader);
    generateModelDataCpp(c, modelDataCpp);
    delete c;
}


