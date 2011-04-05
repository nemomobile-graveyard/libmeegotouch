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
               bool generateDestructor,
               bool internal,
               const QList<ModelProperty>& properties) :
        m_name(name),
        m_superClassName(superClassName),
        m_generatePrivate(generatePrivate),
        m_generateDestructor(generateDestructor),
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
    bool generateDestructor() const {
        return m_generateDestructor;
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
    bool m_generateDestructor;
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
    QRegExp classRegExp("\\s*class\\s+(\\S+\\s+)?(\\w+)\\s*:\\s*public\\s+(\\w+)\\s*");
    int index = line.indexOf(classRegExp);
    if (index == -1) {
        return NULL;
    }

    index += classRegExp.matchedLength();
    className = classRegExp.cap(2);
    superClassName = classRegExp.cap(3);

    // search "M_STYLE(XX)" or "M_STYLE_INTERNAL(XX)"
    QRegExp styleRegExp("\\s*M_STYLE\\s*\\(\\s*\\w+\\s*\\)\\s*");
    QRegExp styleInternalRegExp("\\s*M_STYLE_INTERNAL\\s*\\(\\s*\\w+\\s*\\)\\s*");

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
    // search all "M_STYLE_ATTRIBUTE(A,B,C)"
    QRegExp attributeRegExp("\\s*M_STYLE_ATTRIBUTE\\s*\\(\\s*(\\w+\\:*\\w*)\\s*,\\s*(\\w+)\\s*,\\s*(\\w+)\\s*\\)\\s*");
    QRegExp attributePtrRegExp("\\s*M_STYLE_PTR_ATTRIBUTE\\s*\\(\\s*(\\w+\\:*\\w*\\s*\\*+)\\s*,\\s*(\\w+)\\s*,\\s*(\\w+)\\s*\\)\\s*");
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

    QRegExp modeRegExp("\\s*M_STYLE_MODE\\s*\\(\\s*(\\w+)\\s*\\)\\s*");
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
    bool generateDestructor = true;
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
        "(?:\\S+)" + "{,1}" +
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

    // search "M_MODEL(XX)" or "M_MODEL_INTERNAL(XX)" or
    // "M_MODEL_WITH_PRIVATE(XX)" or "M_MODEL_INTERNAL_WITH_PRIVATE(XX)"
    QRegExp modelRegExp(
        spaces + "M_MODEL" + spaces + parenO + spaces + typeName + spaces + parenC + spaces
    );
    QRegExp modelInternal(
        spaces + "M_MODEL_INTERNAL" + spaces + parenO + spaces + typeName + spaces + parenC + spaces
    );
    QRegExp modelWithPrivateRegExp(
        spaces + "M_MODEL_WITH_PRIVATE" + spaces + parenO + spaces + typeName + spaces + parenC + spaces
    );
    QRegExp modelInternalWithPrivateRegExp(
        spaces + "M_MODEL_INTERNAL_WITH_PRIVATE" + spaces + parenO + spaces + typeName + spaces + parenC + spaces
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
    // search all " M_MODEL_PROPERTY(A,B,C,D,E)"

    QRegExp propertyRegExp(
        spaces +
        "M_MODEL_PROPERTY" +
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
        "M_MODEL_PTR_PROPERTY" +
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
    QRegExp destructorRegExp(
        "M_MODEL_CUSTOM_DESTRUCTOR"
    );

    if(line.indexOf(destructorRegExp, index) != -1)
        generateDestructor = false;
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

    return new ModelClass(className, superClassName, generatePrivate, generateDestructor, internal, properties);
}

//////////////////////////
// generate styledata.h //
//////////////////////////
void generateStyleDataHeader(const StyleClass *c, const QList<QString>& modes, const QString headerfilename)
{
    // #ifndef MLABELSTYLEDATA_H
    // #define MLABELSTYLEDATA_H
    //
    // ---- if(internal) ----
    // ---- if(QFile::exists("mwidgetstyledata.h")) ----
    // #include "mwidgetstyledata.h"
    // ---- else                                      ----
    // #include "gen_mwidgetstyledata.h"
    //
    // ---- endif                                     ----
    // ---- endif        ----
    // #include "mlabelstyle.h"
    // ---- if(internal) ----
    // class MLabelStyleData : public MWidgetStyleData
    // ---- else         ----
    // class MLabelStyleData
    // ---- endif        ----
    // {
    //     friend class MLabelStyle;
    // public:
    //     MLabelStyleData();
    //     virtual ~MLabelStyleData() {}
    // private:
    //     QFont font;
    //     QColor color;
    // };
    //
    // ---- if(internal) ----
    // class MLabelStyleContainerPrivate : MWidgetStyleContainer
    // ---- else         ----
    // class MLabelStyleContainerPrivate
    // ---- endif        ----
    // {
    //     friend class MLabelContainer;
    // public:
    //     MLabelContainerPrivate();
    //     virtual ~MStyleContainerPrivate();
    // private:
    //     const MStyle* activeStyle[2];
    //     const MStyle* disabledStyle[2];
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
    if (c->superClassName() == QString("MStyle") && c->internal())
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

    if (c->name() == QString("MStyle")) {
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

    if (c->name() != QString("MStyle")) {
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
            h << "    const MStyle* _" << iterator->toLower() << "Style[2];\n";
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
    // #include "mlabelstyle.h"
    // #include "gen_mlabelstyledata.h";
    // #include "mstylecreator.h"
    //
    // M_REGISTER_STYLE(MLabelStyle)
    //
    // MLabelStyleData::MLabelStyleData()
    // {
    // ---- all pointer attributes set to 0.
    // }
    //
    // MLabelStyle::MLabelStyle() :
    // ---- if(internal) ----
    //     MWidgetStyle(new MButtonModelData)
    // ---- else         ----
    //     MWidgetStyle(),
    //     data(new MLabelStyleData)
    // ---- endif        ----
    // {
    // }
    //
    // MLabelStyle::~MLabelStyle()
    // {
    // ---- if(!internal) ----
    //     delete data;
    // ---- endif         ----
    // }
    //
    // MLabelStyle::MLabelStyle(MLabelStyleData* data, MButtonModelPrivate* dd) :
    // ---- if(internal) ----
    //     MWidgetModel(data, dd == NULL ? new MButtonModelPrivate : dd)
    // ---- else         ----
    //     MWidgetModel(),
    //     data(data),
    //     d_ptr(dd == NULL ? new MButtonModelData : dd)
    // ---- endif        ----
    // {
    // }
    //
    // const QFont& MLabelStyle::font() const
    // {
    //     return static_cast<const MLabelStyleData*>(data)->font;
    // }
    //
    // void MButtonModel::setText(const QFont& font)
    // {
    //     if(static_cast<MLabelStyleData*>(data)->font != font) {
    //         static_cast<MLabelStyleData*>(data)->font = font;
    //         memberModified(Text);
    //     }
    // }
    //
    // ///////////////
    // // CONTAINER //
    // ///////////////
    //
    // MLabelStyleContainerPrivate::MLabelStyleContainerPrivate() :
    //     activeStyle(0),
    //     disabledStyle(0)
    // {
    //     activeStyle[M::Landscape] = NULL;
    //     activeStyle[M::Portrait] = NULL;
    //     disabledStyle[M::Landscape] = NULL;
    //     disabledStyle[M::Portrait] = NULL;
    // }
    //
    // MStyleContainerPrivate::~MStyleContainerPrivate()
    // {
    //     MTheme::releaseStyle(activeStyle[M::Landscape]);
    //     MTheme::releaseStyle(activeStyle[M::Portrait]);
    //     MTheme::releaseStyle(disabledStyle[M::Landscape]);
    //     MTheme::releaseStyle(disabledStyle[M::Portrait]);
    // }
    //
    // MLabelStyleContainer::MLabelStyleContainer() :
    // ---- if(internal) ----
    //     MStyleContainer(new MLabelStyleContainerPrivate)
    // ---- else         ----
    //     MStyleContainer(),
    //     d_ptr(new MLabelStyleContainerPrivate)
    // ---- endif        ----
    // {
    // }
    //
    // MLabelStyleContainer::MLabelStyleContainer(MLabelStyleContainerPrivate* dd) :
    // ---- if(internal) ----
    //     MStyleContainer(dd)
    // ---- else         ----
    //     MStyleContainer(),
    //     d_ptr(dd)
    // ---- endif        ----
    // {
    // }
    //
    // MLabelStyleContainer::~MLabelStyleContainer()
    // {
    // ---- if(!internal) ----
    //     delete d_ptr;
    // ---- endif         ----
    // }
    //
    // const MLabelStyle* MLabelStyleContainer::operator->() const
    // {
    //     return static_cast<const MLabelStyle*>(currentStyle());
    // }
    //
    // void MLabelStyleContainer::reloadStyles()
    // {
    //     MWidgetStyleContainer::reloadStyles();
    //     Q_D(MLabelStyleContainer);
    //     const MStyle* tmp;
    //     tmp = d->activeStyle[M::Landscape];
    //     d->activeStyle[M::Landscape] = MTheme::style(styleType(), objectName(), "active", M::Landscape, parent());
    //     MTheme::releaseStyle(tmp);
    //     tmp = d->activeStyle[M::Portrait];
    //     d->activeStyle[M::Portrait] = MTheme::style(styleType(), objectName(), "active", M::Portrait, parent());
    //     MTheme::releaseStyle(tmp);
    //
    //     tmp = d->disabledStyle[M::Landscape];
    //     d->disabledStyle[M::Landscape] = MTheme::style(styleType(), objectName(), "disabled", M::Landscape, parent());
    //     MTheme::releaseStyle(tmp);
    //     tmp = d->disabledStyle[M::Portrait];
    //     d->disabledStyle[M::Portrait] = MTheme::style(styleType(), objectName(), "disabled", M::Portrait, parent());
    //     MTheme::releaseStyle(tmp);
    // }
    //
    // const char* MLabelStyleContainer::styleType() const
    // {
    //     return "MLabelStyle";
    // }
    //
    // void MLabelStyleContainer::setModeActive()
    // {
    //     Q_D(MLabelStyleContainer);
    //     setCurrentStyle(d->activeStyle[M::Landscape], d->activeStyle[M::Portrait]);
    // }
    //
    // void MLabelStyleContainer::setModeDisabled()
    // {
    //     Q_D(MLabelStyleContainer);
    //     setCurrentStyle(d->disabledStyle[M::Landscape], d->disabledStyle[M::Portrait]);
    // }

    QFile file(cppfilename);
    if (!file.open(QFile::WriteOnly))
        qFatal("Could not open data cpp for writing");

    QTextStream cpp(&file);


    cpp << "#include \"" << c->name().toLower() << ".h\"\n";
    cpp << "#include \"gen_" << c->name().toLower() << "data.h\"\n";
    cpp << "#include \"mstylecreator.h\"\n";
    cpp << "#include \"mtheme.h\"\n\n";

    cpp << "M_REGISTER_STYLE(" << c->name() << ")\n\n";

    // public constructor of Data class.
    cpp << c->name() << "Data::" << c->name() << "Data()\n";

    bool initializerListStarted = false;
    if (c->name() == QString("MStyle")) {
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

    if (c->name() != QString("MStyle")) {
        cpp << "///////////////\n";
        cpp << "// CONTAINER //\n";
        cpp << "///////////////\n\n";

        // private class constructor
        cpp << c->name() << "ContainerPrivate::" << c->name() << "ContainerPrivate()\n";

        cpp << "{\n";
        QList<QString>::const_iterator iterator = modes.constBegin();
        for (; iterator != modes.constEnd(); ++iterator) {
            cpp << "    _" << iterator->toLower() << "Style[M::Landscape] = NULL;\n";
            cpp << "    _" << iterator->toLower() << "Style[M::Portrait] = NULL;\n";
        }
        cpp << "}\n\n";

        // private class destructor
        cpp << c->name() << "ContainerPrivate::~" << c->name() << "ContainerPrivate()\n";
        cpp << "{\n";
        iterator = modes.begin();
        for (; iterator != modes.end(); ++iterator) {
            cpp << "    MTheme::releaseStyle(_" << iterator->toLower() << "Style[M::Landscape]);\n";
            cpp << "    MTheme::releaseStyle(_" << iterator->toLower() << "Style[M::Portrait]);\n";
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

        // _dummyNeverToBeUsed
        cpp << c->name() << "Container::dummyNeverToBeUsedPtr " << c->name() << "Container::_dummyNeverToBeUsed()\n";
        cpp << "{\n";
        cpp << "    return &" << c->name() << "Container::operator->;\n";
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
            QString mode;
            QString::const_iterator i = iterator->begin(),
                                    e = iterator->end();
            mode += i->toLower();
            ++i;
            for(; i != e; ++i) {
                if(i->isUpper())
                    mode += QString('-') + i->toLower();
                else
                    mode += *i;
            }
            cpp << "    setCurrentMode(\"" << mode << "\");\n";
            cpp << "}\n\n";
        }
    }
}

//////////////////////////
// generate modeldata.h //
//////////////////////////
void generateModelDataHeader(const ModelClass *c, const QString &headerfilename)
{
    // #ifndef MBUTTONMODELDATA_H
    // #define MBUTTONMODELDATA_H
    //
    // ---- if(internal) ----
    // ---- if(QFile::exists("mwidgetmodeldata.h")) ----
    // #include "mwidgetmodeldata.h"
    // ---- else                                      ----
    // #include "gen_mwidgetmodeldata.h"
    // ---- endif                                     ----
    // ---- endif        ----
    // #include "mbuttonmodel.h"
    //
    // ---- if(internal) ----
    // class MButtonModelData : public MWidgetModelData
    // ---- else         ----
    // class MButtonModelData
    // ---- endif        ----
    // {
    //     friend class MButtonModel;
    // public:
    //     virtual ~MButtonModelData() {}
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
    // #include "mbuttonmodel.h"
    // ---- if(generatePrivate) ----
    // #include "mbuttonmodel_p.h"
    // ---- endif               ----
    // #include "gen_mbuttonmodeldata.h";
    //
    // const char* const MButtonModel::Text = "text";
    // const char* const MButtonModel::IconID = "iconID";
    //
    // MButtonModel::MButtonModel() :
    // ---- if(generatePrivate && internal) ----
    //     MWidgetModel(new MButtonModelData, new MButtonModelPrivate)
    // ---- else if(generatePrivate)        ----
    //     MWidgetModel(),
    //     data(new MButtonModelData),
    //     d_ptr(new MButtonModelPrivate)
    // ---- else if(internal)               ----
    //     MWidgetModel(new MButtonModelData)
    // ---- else                            ----
    //     MWidgetModel(),
    //     data(new MButtonModelData)
    // ---- endif                           ----
    // {
    //     static_cast<MButtonModelData*>(data)->text = "DEFAULT_A";
    //     static_cast<MButtonModelData*>(data)->iconID = "DEFAULT_B";
    // }
    //
    // ---- if(generateDestructor) ----
    // MButtonModel::~MButtonModel()
    // {
    // ---- if(!internal)                    ----
    //     delete data;
    // ---- if(generatePrivate && !internal) ----
    //     delete d_ptr;
    // ---- endif                            ----
    // }
    // ---- endif        ----
    //
    // ---- if(generatePrivate) ----
    // MButtonModel::MButtonModel(MButtonModelData* data, MButtonModelPrivate* dd) :
    // ---- if(internal) ----
    //     MWidgetModel(data, dd == NULL ? new MButtonModelPrivate : dd)
    // ---- else         ----
    //     MWidgetModel(),
    //     data(data),
    //     d_ptr(dd == NULL ? new MButtonModelData : dd)
    // ---- endif        ----
    // {
    //     static_cast<MButtonModelData*>(data)->text = "DEFAULT_A";
    //     static_cast<MButtonModelData*>(data)->iconID = "DEFAULT_B";
    // }
    // ---- else                ----
    // MButtonModel::MButtonModel(MButtonModelData* data) :
    // ---- if(internal) ----
    //     MWidgetModel(data)
    // ---- else         ----
    //     MWidgetModel(),
    //     data(data)
    // {
    // }
    // ---- endif               ----
    //
    // QString& MButtonModel::_text()
    // {
    //     return static_cast<MButtonModelData*>(data)->text;
    // }
    //
    // const QString& MButtonModel::_text() const
    // {
    //     return static_cast<const MButtonModelData*>(data)->text;
    // }
    //
    // const QString& MButtonModel::text() const
    // {
    //     return static_cast<const MButtonModelData*>(data)->text;
    // }
    //
    // void MButtonModel::setText(const QString& text)
    // {
    //     if(static_cast<MButtonModelData*>(data)->text != text) {
    //         static_cast<MButtonModelData*>(data)->text = text;
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
            // 5th M_MODEL_PROPERTY() parameter optional, but that is difficult
            // because it is also a C macro.
            const QString defaultValue = i->defaultValue();
            if (!defaultValue.isEmpty() && (defaultValue != "QString()") && (defaultValue != "QString::null")) {
                cpp << "    static_cast<" << c->name() << "Data*>(data)->" << i->name() << " = " << defaultValue << ";\n";
            }
        }
    }
    cpp << "}\n\n";

    // destructor
    if (c->generateDestructor()) {
        cpp << c->name() << "::~" << c->name() << "()\n";
        cpp << "{\n";
        if (!c->internal()) {
            cpp << "    delete data;\n";
            if (c->generatePrivate()) {
                cpp << "    delete d_ptr;\n";
            }
        }
        cpp << "}\n\n";
    }

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
            // 5th M_MODEL_PROPERTY() parameter optional, but that is difficult
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


