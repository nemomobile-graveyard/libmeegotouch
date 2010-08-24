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

#include <QString>
#include <QMetaObject>
#include <QDataStream>
#include <QMetaProperty>
#include <QVariant>
#include <QFont>
#include <QDebug>
#include <QTextCharFormat>
#include <QEasingCurve>

#include "mstylesheet.h"
#include "mstyle.h"
#include "mdeviceprofile.h"
#include "mtheme.h"
#include "mfeedback.h"
#include "mapplication.h"
#include "mapplicationwindow.h"
#include "mstylesheetparser.h"
#include "mshareddata.h"
#include "mbackgroundtiles.h"

// internal enum that defines the attribute type
typedef enum {
    BOOL_TYPE,
    INT_TYPE,
    COLOR_TYPE,
    REAL_TYPE,
    CONST_PIXMAP_TYPE,
    CONST_SCALABLE_TYPE,
    SCALABLE_IMAGE_TILES_TYPE,
    SIZE_TYPE,
    SIZEF_TYPE,
    POINT_TYPE,
    POINTF_TYPE,
    FONT_TYPE,
    STRING_TYPE,
    CHAR_TYPE,
    ALIGNMENT_TYPE,
    ORIENTATION_TYPE,
    UNDERLINESTYLE_TYPE,
    PENSTYLE_TYPE,
    AXIS_TYPE,
    FEEDBACK_TYPE,

#if QT_VERSION >= 0x040600
    EASINGCURVE_TYPE,
#endif

    CONST_PIXMAP_TYPE_0,
    CONST_PIXMAP_TYPE_1,
    CONST_PIXMAP_TYPE_3,

    CONST_SCALABLE_TYPE_0,
    CONST_SCALABLE_TYPE_1,
    CONST_SCALABLE_TYPE_5,

    NUM_TYPES

} MContainerType;

// types of attribute values in CSS file
enum {
    COLOR_VALUE,
    IMAGE_VALUE,
    BOX_VALUE,
    SCALABLE_VALUE,
    SIZE_VALUE,
    POS_VALUE,
    FONT_VALUE,

    NUM_VALUES
};

// measurement unit types
enum {
    PIXELS_UNIT,
    MM_UNIT,
    PERCENT_UNIT,

    NUM_UNITS
};

static const QString units[NUM_UNITS] = {
    QString("px"),
    QString("mm"),
    QString('%')
};

static const QString types[NUM_TYPES] = {
    QString("bool"),
    QString("int"),
    QString("QColor"),
    QString("qreal"),
    QString("const QPixmap*"),
    QString("const MScalableImage*"),
    QString("MBackgroundTiles"),
    QString("QSize"),
    QString("QSizeF"),
    QString("QPoint"),
    QString("QPointF"),
    QString("QFont"),
    QString("QString"),
    QString("QChar"),
    QString("Qt::Alignment"),
    QString("Qt::Orientation"),
    QString("QTextCharFormat::UnderlineStyle"),
    QString("Qt::PenStyle"),
    QString("Qt::Axis"),
    QString("MFeedback"),
#if QT_VERSION >= 0x040600
    QString("QEasingCurve")
#endif
};

static const QString values[NUM_VALUES] = {
    QString('#'),
    QString("image("),
    QString("box("),
    QString("scalable("),
    QString("size("),
    QString("pos("),
    QString("font("),
};

Q_DECLARE_METATYPE(const QPixmap *)
Q_DECLARE_METATYPE(const MScalableImage *)
Q_DECLARE_METATYPE(MBackgroundTiles)
Q_DECLARE_METATYPE(QTextCharFormat::UnderlineStyle)
Q_DECLARE_METATYPE(Qt::PenStyle)
Q_DECLARE_METATYPE(Qt::Axis)
Q_DECLARE_METATYPE(QPoint)
Q_DECLARE_METATYPE(QPointF)
Q_DECLARE_METATYPE(QSize)
Q_DECLARE_METATYPE(QSizeF)
Q_DECLARE_METATYPE(MFeedback)

#if QT_VERSION >= 0x040600
#include <QEasingCurve>
Q_DECLARE_METATYPE(QEasingCurve)
#endif

//! \internal
class QtDatatypeConverter
{
public:
    QMap<QString, Qt::Alignment> ALIGNMENTS;
    QMap<QString, Qt::Orientation> ORIENTATIONS;
    QMap<QString, QTextCharFormat::UnderlineStyle> UNDERLINESTYLES;
    QMap<QString, Qt::PenStyle> PENSTYLES;
    QMap<QString, Qt::Axis> AXES;
    QMap<QString, QFont::Weight> WEIGHTS;
    QMap<QString, QFont::Capitalization> CAPITALIZATION;
#if QT_VERSION >= 0x040600
    QMap<QString, QEasingCurve::Type> EASINGCURVETYPES;
#endif

    QtDatatypeConverter() {
        ALIGNMENTS["left"] = Qt::AlignLeft;
        ALIGNMENTS["right"] = Qt::AlignRight;
        ALIGNMENTS["hcenter"] = Qt::AlignHCenter;
        ALIGNMENTS["justify"] = Qt::AlignJustify;
        ALIGNMENTS["top"] = Qt::AlignTop;
        ALIGNMENTS["bottom"] = Qt::AlignBottom;
        ALIGNMENTS["vcenter"] = Qt::AlignVCenter;
        ALIGNMENTS["center"] = Qt::AlignCenter;
        ALIGNMENTS["absolute"] = Qt::AlignAbsolute;

        ORIENTATIONS["vertical"] = Qt::Vertical;
        ORIENTATIONS["horizontal"] = Qt::Horizontal;

        UNDERLINESTYLES["nounderline"] = QTextCharFormat::NoUnderline;
        UNDERLINESTYLES["singleunderline"] = QTextCharFormat::SingleUnderline;
        UNDERLINESTYLES["dashunderline"] = QTextCharFormat::DashUnderline;
        UNDERLINESTYLES["dotline"] = QTextCharFormat::DotLine;
        UNDERLINESTYLES["dashdotline"] = QTextCharFormat::DashDotLine;
        UNDERLINESTYLES["dashdotdotline"] = QTextCharFormat::DashDotDotLine;
        UNDERLINESTYLES["waveunderline"] = QTextCharFormat::WaveUnderline;

        PENSTYLES["nopen"] = Qt::NoPen;
        PENSTYLES["solidline"] = Qt::SolidLine;
        PENSTYLES["dashline"] = Qt::DashLine;
        PENSTYLES["dotline"] = Qt::DotLine;
        PENSTYLES["dashdotline"] = Qt::DashDotLine;
        PENSTYLES["dashdotdotline"] = Qt::DashDotDotLine;

        AXES["x"] = Qt::XAxis;
        AXES["y"] = Qt::YAxis;
        AXES["z"] = Qt::ZAxis;

        WEIGHTS["light"] = QFont::Light;
        WEIGHTS["normal"] = QFont::Normal;
        WEIGHTS["demibold"] = QFont::DemiBold;
        WEIGHTS["bold"] = QFont::Bold;
        WEIGHTS["black"] = QFont::Black;

        CAPITALIZATION["mixedcase"] = QFont::MixedCase;
        CAPITALIZATION["uppercase"] = QFont::AllUppercase;
        CAPITALIZATION["lowercase"] = QFont::AllLowercase;
        CAPITALIZATION["smallcaps"] = QFont::SmallCaps;
        CAPITALIZATION["capitalize"] = QFont::Capitalize;

#if QT_VERSION >= 0x040600
        EASINGCURVETYPES["linear"] = QEasingCurve::Linear;
        EASINGCURVETYPES["inquad"] = QEasingCurve::InQuad;
        EASINGCURVETYPES["outquad"] = QEasingCurve::OutQuad;
        EASINGCURVETYPES["inoutquad"] = QEasingCurve::InOutQuad;
        EASINGCURVETYPES["outinquad"] = QEasingCurve::OutInQuad;
        EASINGCURVETYPES["incubic"] = QEasingCurve::InCubic;
        EASINGCURVETYPES["outcubic"] = QEasingCurve::OutCubic;
        EASINGCURVETYPES["inoutcubic"] = QEasingCurve::InOutCubic;
        EASINGCURVETYPES["outincubic"] = QEasingCurve::OutInCubic;
        EASINGCURVETYPES["inquart"] = QEasingCurve::InQuart;
        EASINGCURVETYPES["outquart"] = QEasingCurve::OutQuart;
        EASINGCURVETYPES["inoutquart"] = QEasingCurve::InOutQuart;
        EASINGCURVETYPES["outinquart"] = QEasingCurve::OutInQuart;
        EASINGCURVETYPES["inquint"] = QEasingCurve::InQuint;
        EASINGCURVETYPES["outquint"] = QEasingCurve::OutQuint;
        EASINGCURVETYPES["inoutquint"] = QEasingCurve::InOutQuint;
        EASINGCURVETYPES["outinquint"] = QEasingCurve::OutInQuint;
        EASINGCURVETYPES["insine"] = QEasingCurve::InSine;
        EASINGCURVETYPES["outsine"] = QEasingCurve::OutSine;
        EASINGCURVETYPES["inoutsine"] = QEasingCurve::InOutSine;
        EASINGCURVETYPES["outinsine"] = QEasingCurve::OutInSine;
        EASINGCURVETYPES["inexpo"] = QEasingCurve::InExpo;
        EASINGCURVETYPES["outexpo"] = QEasingCurve::OutExpo;
        EASINGCURVETYPES["inoutexpo"] = QEasingCurve::InOutExpo;
        EASINGCURVETYPES["outinexpo"] = QEasingCurve::OutInExpo;
        EASINGCURVETYPES["incirc"] = QEasingCurve::InCirc;
        EASINGCURVETYPES["outcirc"] = QEasingCurve::OutCirc;
        EASINGCURVETYPES["inoutcirc"] = QEasingCurve::InOutCirc;
        EASINGCURVETYPES["outincirc"] = QEasingCurve::OutInCirc;
        EASINGCURVETYPES["inelastic"] = QEasingCurve::InElastic;
        EASINGCURVETYPES["outelastic"] = QEasingCurve::OutElastic;
        EASINGCURVETYPES["inoutelastic"] = QEasingCurve::InOutElastic;
        EASINGCURVETYPES["outinelastic"] = QEasingCurve::OutInElastic;
        EASINGCURVETYPES["inback"] = QEasingCurve::InBack;
        EASINGCURVETYPES["outback"] = QEasingCurve::OutBack;
        EASINGCURVETYPES["inoutback"] = QEasingCurve::InOutBack;
        EASINGCURVETYPES["outinback"] = QEasingCurve::OutInBack;
        EASINGCURVETYPES["inbounce"] = QEasingCurve::InBounce;
        EASINGCURVETYPES["outbounce"] = QEasingCurve::OutBounce;
        EASINGCURVETYPES["inoutbounce"] = QEasingCurve::InOutBounce;
        EASINGCURVETYPES["outinbounce"] = QEasingCurve::OutInBounce;
#endif
    }
};
//! \internal_end

static QtDatatypeConverter DataTypeConverter;

int MStyleSheetAttribute::attributeToInt(const QString &attribute, bool *conversionOk)
{
    QString value = attribute.trimmed();

    if (attribute.endsWith(units[PIXELS_UNIT])) {
        // strip "px" from the end
        value.truncate(value.length() - 2);
        return value.toInt(conversionOk);
    }

    if (attribute.endsWith(units[MM_UNIT])) {
        // strip "mm" from the end
        value.truncate(value.length() - 2);

        return MDeviceProfile::instance()->mmToPixels(value.toFloat(conversionOk));
    }

    return value.toInt(conversionOk);
}

int MStyleSheetAttribute::attributeToInt(const QString &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation)
{
    QString value = attribute.trimmed();

    if (attribute.endsWith(units[PERCENT_UNIT])) {
        int maximumValue = 0;

        if (orientation == M::Landscape) {
            if (type == WidthAttribute)
                maximumValue = MDeviceProfile::instance()->resolution().width();
            else
                maximumValue = MDeviceProfile::instance()->resolution().height();
        } else {
            if (type == WidthAttribute)
                maximumValue = MDeviceProfile::instance()->resolution().height();
            else
                maximumValue = MDeviceProfile::instance()->resolution().width();
        }

        value.truncate(value.length() - 1);

        // convert to integer value
        qreal v = value.toFloat(conversionOk);
        return(v / 100.0 * (float)maximumValue);
    } else {
        return attributeToInt(attribute, conversionOk);
    }
}

qreal MStyleSheetAttribute::attributeToFloat(const QString &attribute, bool *conversionOk)
{
    QString value = attribute.trimmed();

    if (attribute.endsWith(units[PIXELS_UNIT])) {
        // strip "px" from the end
        value.truncate(value.length() - 2);
        return value.toFloat(conversionOk);
    }

    if (attribute.endsWith(units[MM_UNIT])) {
        // strip "mm" from the end
        value.truncate(value.length() - 2);

        return MDeviceProfile::instance()->mmToPixelsF(value.toFloat(conversionOk));
    }

    return value.toFloat(conversionOk);
}

qreal MStyleSheetAttribute::attributeToFloat(const QString &attribute, bool *conversionOk, SizeAttributeType type, M::Orientation orientation)
{
    QString value = attribute.trimmed();

    if (attribute.endsWith(units[PERCENT_UNIT])) {
        int maximumValue = 0;

        if (orientation == M::Landscape) {
            if (type == WidthAttribute)
                maximumValue = MDeviceProfile::instance()->resolution().width();
            else
                maximumValue = MDeviceProfile::instance()->resolution().height();
        } else {
            if (type == WidthAttribute)
                maximumValue = MDeviceProfile::instance()->resolution().height();
            else
                maximumValue = MDeviceProfile::instance()->resolution().width();
        }

        value.truncate(value.length() - 1);

        // convert to integer value
        qreal v = value.toFloat(conversionOk);//myAtof(value, conversionOk);
        return(v / 100.0 * (float) maximumValue);
    } else {
        return attributeToFloat(attribute, conversionOk);
    }
}

bool MStyleSheetAttribute::booleanFromString(const QString &string, bool *conversionOk)
{
    if (string.compare("true", Qt::CaseInsensitive) == 0) {
        if (conversionOk)
            *conversionOk = true;
        return true;
    } else if (string.compare("false", Qt::CaseInsensitive) == 0) {
        if (conversionOk)
            *conversionOk = true;
        return false;
    }

    if (conversionOk)
        *conversionOk = false;
    return bool();
}

QColor MStyleSheetAttribute::colorFromString(const QString &string, bool *conversionOk)
{
    QColor color(string);
    if (conversionOk)
        *conversionOk = (string.length() > 0) ? color.isValid() : true;
    return color;
}

QFont MStyleSheetAttribute::fontFromString(const QString string, bool *conversionOk)
{
    //font: "font family" 20px
    //font: "font family" bold 20px
    //font: "font family" italic 20px
    //font: "font family" uppercase 20px
    //font: "font family" bold italic 20px
    //font: "font family" bold uppercase 20px
    //font: "font family" italic uppercase 20px
    //font: "font family" bold italic uppercase 20px
    //font: font_family 20px
    //font: font_family bold 20px
    //font: font_family italic 20px
    //font: font_family uppercase 20px
    //font: font_family bold italic 20px
    //font: font_family bold uppercase 20px
    //font: font_family italic uppercase 20px
    //font: font_family bold italic uppercase 20px
    bool ok;
    if (!conversionOk)
        conversionOk = &ok;
    *conversionOk = false;

    QStringList list;
    if (string.startsWith('\"')) {
        int idx = string.indexOf('\"', 1);
        if (idx != -1) {
            //get quoted font family name
            QString family = string.mid(1, idx - 1);

            //split rest of the parameters
            QString values = string.mid(idx + 1).trimmed();
            list = values.split(' ', QString::SkipEmptyParts);

            //insert family as first parameter
            list.insert(0, family);
        }
    } else {
        //no quotes, just split the parameters
        list = string.split(' ');
    }

    //family + font size
    if (list.size() == 2) {
        qreal pixelSize = attributeToFloat(list[1], conversionOk);
        if (*conversionOk) {
            QFont font(list[0]);
            font.setPixelSize(pixelSize);
            return font;
        }
    }
    //family + weight/italic/capitalization + font size
    else if (list.size() == 3) {
        qreal pixelSize = attributeToFloat(list[2], conversionOk);
        if (*conversionOk) {
            QFont font(list[0]);
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list[1])) {
                font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);
                return font;
            } else if (list[1] == "italic") {
                font.setItalic(true);
                return font;
            } else if (DataTypeConverter.CAPITALIZATION.contains(list[1])) {
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list[1]]);
                return font;
            } else {
                *conversionOk = false;
            }
        }
    }
    //family + weight/italic + italic/capitalization + font size
    else if (list.size() == 4) {
        qreal pixelSize = attributeToFloat(list[3], conversionOk);
        if (*conversionOk) {
            QFont font(list[0]);
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list[1]) && list[2] == "italic") {
                font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);
                font.setItalic(true);
                return font;
            } else if (DataTypeConverter.WEIGHTS.contains(list[1]) && DataTypeConverter.CAPITALIZATION.contains(list[2])) {
                font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list[2]]);
                return font;
            } else if (list[1] == "italic" && DataTypeConverter.CAPITALIZATION.contains(list[2])) {
                font.setItalic(true);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list[2]]);
                return font;
            } else {
                *conversionOk = false;
            }
        }
    }
    //family + weight + italic + capitalization + font size
    else if (list.size() == 5) {
        qreal pixelSize = attributeToFloat(list[4], conversionOk);
        if (*conversionOk) {
            QFont font(list[0]);
            font.setPixelSize(pixelSize);
            if (DataTypeConverter.WEIGHTS.contains(list[1]) && list[2] == "italic" && DataTypeConverter.CAPITALIZATION.contains(list[3])) {
                font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);
                font.setItalic(true);
                font.setCapitalization(DataTypeConverter.CAPITALIZATION[list[3]]);
                return font;
            } else {
                *conversionOk = false;
            }
        }
    }

    return QFont();
}

QString MStyleSheetAttribute::attributeNameToPropertyName(const QString &attributeName) {
    QString result;
    const int length = attributeName.length();
    for (int i = 0; i < length; ++i) {
        if (attributeName.at(i) == '-') {
            if (length > i + 1) {
                i++;
                result += QChar(attributeName.at(i)).toUpper();
            }
        } else {
            result += attributeName.at(i);
        }
    }

    return result;
}

bool MStyleSheetAttribute::writeAttribute(const QString &filename,
        MStyle *style,
        const QMetaProperty &property,
        M::Orientation orientation)
{

    bool conversionOK = false;

    QString attributeType(property.typeName());
    if (attributeType == types[BOOL_TYPE]) {
        bool result = booleanFromString(value, &conversionOK);
        if (conversionOK) {
            return property.write(style, result);
        }
    } else if (attributeType == types[INT_TYPE]) {
        int integer = attributeToInt(value, &conversionOK);
        if (conversionOK) {
            return property.write(style, integer);
        }
    } else if (attributeType == types[COLOR_TYPE]) {
        if(value == "none")
            return property.write(style, QColor());

        QColor color = colorFromString(value, &conversionOK);
        if (conversionOK) {
            return property.write(style, color);
        }
    } else if (attributeType == types[REAL_TYPE]) {
        qreal real = attributeToFloat(value, &conversionOK);
        if (conversionOK) {
            return property.write(style, real);
        }
    } else if (attributeType == types[CONST_PIXMAP_TYPE]) {
        if(value == "none")
            return property.write(style, qVariantFromValue((const QPixmap *) NULL));

        //"image: image_id;"
        //"image: image_id 64px 64px;"
        //"image: "image id";"
        //"image: "image id" 64px 64px;"

        QStringList list;
        if (value.startsWith('\"')) {
            //parse name inside quotes
            int idx = value.indexOf('\"', 1);
            if (idx != -1) {
                //get quoted image_id
                QString imageid = value.mid(1, idx - 1);

                //split rest of the parameters
                QString values = value.mid(idx + 1).trimmed();
                list = values.split(' ', QString::SkipEmptyParts);

                //insert image_id as first parameter
                list.insert(0, imageid);
            }
        } else {
            //no quotes, just split the parameters
            list = value.split(' ');
        }

        //only image_id
        if (list.size() == 1) {
            const QPixmap *pixmap = MTheme::pixmap(list[0]);
            return property.write(style, qVariantFromValue(pixmap));
        }
        //image_id + width + height
        else if (list.size() == 3) {
            int width = attributeToInt(list[1], &conversionOK, WidthAttribute, orientation);
            int height = attributeToInt(list[2], &conversionOK, HeightAttribute, orientation);
            const QPixmap *pixmap = MTheme::pixmap(list[0], QSize(width, height));
            return property.write(style, qVariantFromValue(pixmap));
        }
        //no parameters
        else if (list.size() == 0) {
            //init null pixmap which is ok if someone does not want to use it
            return property.write(style, qVariantFromValue((const QPixmap *) NULL));
        }
    } else if (attributeType == types[CONST_SCALABLE_TYPE] || attributeType == types[SCALABLE_IMAGE_TILES_TYPE]) {
        //"background: image_id left right top bottom;"
        //"background: image_id;"
        //"background: "image id" left right top bottom;"
        //"background: "image id";"

        QStringList list;
        if (value.startsWith('\"')) {
            //parse name inside quotes
            int idx = value.indexOf('\"', 1);
            if (idx != -1) {
                //get quoted image_id
                QString imageid = value.mid(1, idx - 1);

                //split rest of the parameters
                QString values = value.mid(idx + 1).trimmed();
                list = values.split(' ', QString::SkipEmptyParts);

                //insert image_id as first parameter
                list.insert(0, imageid);
            }
        } else {
            //no quotes, just split the parameters
            list = value.split(' ');
        }

        //no parameters
        if (value.isEmpty() || value == "none") {
            //init null image which is ok if someone does not want to use it
            if(attributeType == types[CONST_SCALABLE_TYPE])
                return property.write(style, qVariantFromValue((const MScalableImage *) NULL));
            else
                return property.write(style, QVariant::fromValue(MBackgroundTiles()));
        }
        //only image_id
        else if (list.size() == 1) {
            if(attributeType == types[CONST_SCALABLE_TYPE]) {
                const MScalableImage *image = MTheme::scalableImage(list[0], 0, 0, 0, 0);
                return property.write(style, qVariantFromValue(image));
            } else {
                return property.write(style, QVariant::fromValue(MBackgroundTiles(list[0], 0,0,0,0)));
            }
        }
        //image_id + border width paramaters
        else if (list.size() == 5) {
            //image_id and the border parameters
            if(attributeType == types[CONST_SCALABLE_TYPE]) {
                const MScalableImage *image = MTheme::scalableImage(list[0],
                                                attributeToInt(list[1], &conversionOK),
                                                attributeToInt(list[2], &conversionOK),
                                                attributeToInt(list[3], &conversionOK),
                                                attributeToInt(list[4], &conversionOK));
                return property.write(style, qVariantFromValue(image));
            } else {
                return property.write(style, QVariant::fromValue(MBackgroundTiles(list[0],
                                                                attributeToInt(list[1], &conversionOK),
                                                                attributeToInt(list[2], &conversionOK),
                                                                attributeToInt(list[3], &conversionOK),
                                                                attributeToInt(list[4], &conversionOK))));
            }
        }
    } else if (attributeType == types[SIZE_TYPE] || attributeType == types[SIZEF_TYPE]) {
        //size: 25px 25px;

        //just split into pieces and create QSize or QSizeF depending on the attributeType
        QStringList list = value.split(' ', QString::SkipEmptyParts);
        if (list.size() == 2) {
            if (attributeType == types[SIZE_TYPE]) {
                int width = attributeToInt(list[0], &conversionOK, WidthAttribute, orientation);
                int height = attributeToInt(list[1], &conversionOK, HeightAttribute, orientation);
                return property.write(style, QSize(width, height));
            } else {
                qreal width = attributeToFloat(list[0], &conversionOK, WidthAttribute, orientation);
                qreal height = attributeToFloat(list[1], &conversionOK, HeightAttribute, orientation);
                return property.write(style, QSizeF(width, height));
            }
        }
    } else if (attributeType == types[POINT_TYPE] || attributeType == types[POINTF_TYPE]) {
        //"point: 256px 123px;

        //just split into pieces and create QPoint or QPointF depending on the attributeType
        QStringList list = value.split(' ', QString::SkipEmptyParts);
        if (list.size() == 2) {
            if (attributeType == types[POINT_TYPE]) {
                int x = attributeToInt(list[0], &conversionOK, WidthAttribute, orientation);
                int y = attributeToInt(list[1], &conversionOK, HeightAttribute, orientation);
                return property.write(style, QPoint(x, y));
            } else {
                qreal x = attributeToFloat(list[0], &conversionOK, WidthAttribute, orientation);
                qreal y = attributeToFloat(list[1], &conversionOK, HeightAttribute, orientation);
                return property.write(style, QPointF(x, y));
            }
        }
    } else if (attributeType == types[FONT_TYPE]) {
        QFont font = fontFromString(value, &conversionOK);
        if (conversionOK) {
            return property.write(style, font);
        }
    } else if (attributeType == types[STRING_TYPE]) {
        if (value.length() >= 2) {
            if ((value[0] == 0x22) && (value[value.length()-1] == 0x22)) {
                return property.write(style, value.mid(1, value.length() - 2));
            }
        } else if (value.length() == 0) {
            return property.write(style, QString());
        }
    } else if (attributeType == types[CHAR_TYPE]) {
        if (value.length() == 3) {
            if ((value[0] == QChar('\'')) && (value[2] == QChar('\''))) {
                return property.write(style, static_cast<QChar>(value[1]));
            }
        }
    } else if (attributeType == types[ALIGNMENT_TYPE]) {
        if (DataTypeConverter.ALIGNMENTS.contains(value)) {
            return property.write(style, QVariant(DataTypeConverter.ALIGNMENTS[value]));
        }
    } else if (attributeType == types[ORIENTATION_TYPE]) {
        if (DataTypeConverter.ORIENTATIONS.contains(value)) {
            return property.write(style, QVariant(DataTypeConverter.ORIENTATIONS[value]));
        }
    } else if (attributeType == types[UNDERLINESTYLE_TYPE]) {
        if (DataTypeConverter.UNDERLINESTYLES.contains(value)) {
            return property.write(style, qVariantFromValue(DataTypeConverter.UNDERLINESTYLES[value]));
        }
    } else if (attributeType == types[PENSTYLE_TYPE]) {
        if (DataTypeConverter.PENSTYLES.contains(value)) {
            return property.write(style, qVariantFromValue(DataTypeConverter.PENSTYLES[value]));
        }
    } else if (attributeType == types[AXIS_TYPE]) {
        if (DataTypeConverter.AXES.contains(value)) {
            return property.write(style, qVariantFromValue(DataTypeConverter.AXES[value]));
        }
    } else if (attributeType == types[FEEDBACK_TYPE]) {
        MFeedback feedback(value);
        return property.write(style, qVariantFromValue(feedback));
    }
#if QT_VERSION >= 0x040600
    else if (attributeType == types[EASINGCURVE_TYPE]) {
        QEasingCurve curve;
        // curve type
        QStringList list = value.split(',');
        if (list.size() > 0) {
            if (DataTypeConverter.EASINGCURVETYPES.contains(list[0])) {
                curve.setType(DataTypeConverter.EASINGCURVETYPES[list[0]]);
                // curve amplitude
                if (list.size() > 1) {
                    curve.setAmplitude((qreal) list[1].toDouble());
                    // curve overshoot
                    if (list.size() > 2) {
                        curve.setOvershoot((qreal) list[2].toDouble());
                        // curve period
                        if (list.size() > 3) {
                            curve.setPeriod((qreal) list[3].toDouble());
                        }
                    }
                }
                return property.write(style, qVariantFromValue(curve));
            }
        }
    }
#else
    // This attribute type is supported in Qt4.6->
    return true;
#endif

    MStyleSheetParser::outputParseError(filename, "Not a valid attribute(" + attributeType + "): " + name + ": " + value, MStyleSheetParser::getLineNum(filename, position));
    return false;
}

bool MStyleSheetAttribute::writeAttributeShm(MStyle *style,
        QMetaProperty property,
        MSharedData &ds,
        M::Orientation orientation)
{
    int containerType;
    bool conversionOK = false;

    ds >> containerType;

    if (containerType == REAL_TYPE) {
        qreal tmp;
        ds >> tmp;
        return property.write(style, tmp);

    }  else if (containerType == CONST_PIXMAP_TYPE_0) {
        return property.write(style, qVariantFromValue((const QPixmap *) NULL));

    } else if (containerType == CONST_PIXMAP_TYPE_1) {
        QString name;
        ds >> name;

        const QPixmap *pixmap = MTheme::pixmap(name);
        return property.write(style, qVariantFromValue(pixmap));

    } else if (containerType == CONST_PIXMAP_TYPE_3) {
        QString name;
        int width, height;
        ds >> name >> width >> height;

        const QPixmap *pixmap = MTheme::pixmap(name, QSize(width, height));
        return property.write(style, qVariantFromValue(pixmap));

    }  else if (containerType == CONST_SCALABLE_TYPE_0) {
        property.write(style, qVariantFromValue((const MScalableImage *) NULL));

    }  else if (containerType == CONST_SCALABLE_TYPE_1) {
        QString name;
        ds >> name;
        const MScalableImage *image = MTheme::scalableImage(name, 0, 0, 0, 0);
        return property.write(style, qVariantFromValue(image));

    }  else if (containerType == CONST_SCALABLE_TYPE_5) {
        QString name;
        int left, right, top, bottom;
        ds >> name >> left >> right >> top >> bottom;

        const MScalableImage *image = MTheme::scalableImage(name, left, right, top, bottom);
        return property.write(style, qVariantFromValue(image));

    } else if (containerType == FONT_TYPE) {
        QFont font;
        ds >> font;
        return property.write(style, font);

    } else if (containerType == FEEDBACK_TYPE) {
        QString feedbackName;
        ds >> feedbackName;
        MFeedback feedback(feedbackName);
        return property.write(style, qVariantFromValue(feedback));

    } else if (containerType == ALIGNMENT_TYPE ||
               containerType == ORIENTATION_TYPE ||
               containerType == UNDERLINESTYLE_TYPE ||
               containerType == PENSTYLE_TYPE ||
               containerType == AXIS_TYPE) {
        int value;
        ds >> value;
        return property.write(style, value);

    } else if (containerType == SIZE_TYPE) {
        QString widthStr, heightStr;
        ds >> widthStr >> heightStr;

        int width = MStyleSheetAttribute::attributeToInt(widthStr, &conversionOK, WidthAttribute, orientation);
        int height = MStyleSheetAttribute::attributeToInt(heightStr, &conversionOK, HeightAttribute, orientation);
        return property.write(style, QSize(width, height));

    } else if (containerType == SIZEF_TYPE) {
        QString widthStr, heightStr;
        ds >> widthStr >> heightStr;

        qreal width = MStyleSheetAttribute::attributeToFloat(widthStr, &conversionOK, WidthAttribute, orientation);
        qreal height = MStyleSheetAttribute::attributeToFloat(heightStr, &conversionOK, HeightAttribute, orientation);

        return property.write(style, QSizeF(width, height));

    } else if (containerType == BOOL_TYPE ||
               containerType == INT_TYPE ||
               containerType == COLOR_TYPE ||
               containerType == POINT_TYPE ||
               containerType == POINTF_TYPE ||
               containerType == STRING_TYPE) {
        QVariant tmp;
        ds >> tmp;

        return property.write(style, tmp);
    }

#if QT_VERSION >= 0x040600
    else if (containerType == EASINGCURVE_TYPE) {
        QVariant tmp;
        ds >> tmp;
        return property.write(style, tmp);
    }
#else
    else {
        // This attribute type is supported in Qt4.6->
        return true;
    }
#endif
    return false;
}

void MStyleSheetAttribute::writeAttribute(const QMetaProperty &property,
        MSharedData &shm,
        QString filename)
{
    bool conversionOK = false;

    QString attributeType(property.typeName());

    if (attributeType == types[BOOL_TYPE]) {
        if (value.compare("true", Qt::CaseInsensitive) == 0) {
            shm << (int)BOOL_TYPE << qVariantFromValue(true);
        } else if (value.compare("false", Qt::CaseInsensitive) == 0) {
            shm << (int)BOOL_TYPE << qVariantFromValue(false);
        }
    } else if (attributeType == types[INT_TYPE]) {
        int integer = MStyleSheetAttribute::attributeToInt(value, &conversionOK);
        if (conversionOK)
            shm << (int)INT_TYPE << qVariantFromValue(integer);
    } else if (attributeType == types[COLOR_TYPE]) {
        if (value.length() > 0) {
            QColor color(value);
            if (color.isValid())
                shm << (int)COLOR_TYPE << qVariantFromValue(color);
        } else
            shm << (int)COLOR_TYPE << qVariantFromValue(QColor());
    } else if (attributeType == types[REAL_TYPE]) {
        qreal real = MStyleSheetAttribute::attributeToFloat(value, &conversionOK);

        if (conversionOK)
            shm << (int)REAL_TYPE << real;
    } else if (attributeType == types[CONST_PIXMAP_TYPE]) {
        if (value.startsWith(values[IMAGE_VALUE], Qt::CaseInsensitive)) {
            //"image: image(image_id)"
            //"image: image(image_id, 64px, 64px);"

            QString values = value.mid(6, value.length() - 7);
            QStringList list = values.split(',');
            if (list.size() == 3) {
                //since 0.11
                qWarning("DEPRECATED: using \"image\" prefix for pixmap attributes is deprecated. Use following syntax instead:");
                qWarning("%s: %s; => %s: \"%s\" %s %s; //<-- %s:%d", name.toLatin1().trimmed().data(), value.toLatin1().data(), name.toLatin1().trimmed().data(), list[0].toLatin1().trimmed().data(), list[1].toLatin1().trimmed().data(), list[2].toLatin1().trimmed().data(), filename.toLatin1().data(), MStyleSheetParser::getLineNum(filename, position));

                // Image with size parameters
                QString name = list[0];
                int width = MStyleSheetAttribute::attributeToInt(list[1], &conversionOK);
                int height = MStyleSheetAttribute::attributeToInt(list[2], &conversionOK);

                shm << (int)CONST_PIXMAP_TYPE_3 << name << width << height;
            }

            else if (list.size() == 1) {
                //since 0.11
                qWarning("DEPRECATED: using \"image\" prefix for pixmap attributes is deprecated. Use following syntax instead:");
                qWarning("%s: %s; => %s: \"%s\"; //<-- %s:%d", name.toLatin1().trimmed().data(), value.toLatin1().data(), name.toLatin1().trimmed().data(), list[0].toLatin1().trimmed().data(), filename.toLatin1().data(), MStyleSheetParser::getLineNum(filename, position));

                // Image without size specified
                QString name = list[0];
                shm << (int)CONST_PIXMAP_TYPE_1 << name;
            }
        } else {
            //"image: image_id;"
            //"image: image_id 64px 64px;"
            //"image: "image id";"
            //"image: "image id" 64px 64px;"

            QStringList list;
            if (value.startsWith('\"')) {
                //parse name inside quotes
                int idx = value.indexOf('\"', 1);
                if (idx != -1) {
                    //get quoted image_id
                    QString imageid = value.mid(1, idx - 1);

                    //split rest of the parameters
                    QString values = value.mid(idx + 1).trimmed();
                    list = values.split(' ', QString::SkipEmptyParts);

                    //insert image_id as first parameter
                    list.insert(0, imageid);
                }
            } else {
                //no quotes, just split the parameters
                list = value.split(' ');
            }

            //only image_id
            if (list.size() == 1)
                shm << (int)CONST_PIXMAP_TYPE_1 << list[0];

            //image_id + width + height
            else if (list.size() == 3) {
                int width = MStyleSheetAttribute::attributeToInt(list[1], &conversionOK);
                int height = MStyleSheetAttribute::attributeToInt(list[2], &conversionOK);
                shm << (int)CONST_PIXMAP_TYPE_3 << list[0] << width << height;
            }
            //no parameters
            else if (list.size() == 0) {
                //init null pixmap which is ok if someone does not want to use it
                shm << (int)CONST_PIXMAP_TYPE_0;
            }
        }
    } else if (attributeType == types[CONST_SCALABLE_TYPE]) {
        if (value.startsWith(values[SCALABLE_VALUE], Qt::CaseInsensitive)) {
            //"background: scalable(image_id, left, right, top, bottom);"

            QString values = value.mid(9, value.length() - 10);
            QStringList list = values.split(',');
            if (list.size() == 5) {
                QString name = list[0];
                int left = MStyleSheetAttribute::attributeToInt(list[1], &conversionOK);
                int right = MStyleSheetAttribute::attributeToInt(list[2], &conversionOK);
                int top = MStyleSheetAttribute::attributeToInt(list[3], &conversionOK);
                int bottom = MStyleSheetAttribute::attributeToInt(list[4], &conversionOK);

                //since 0.11
                qWarning("DEPRECATED: using \"scalable\" prefix for images is deprecated. Use following syntax instead:");
                qWarning("%s: %s; => %s: \"%s\" %s %s %s %s; //<-- %s:%d", name.toLatin1().trimmed().data(), value.toLatin1().data(), name.toLatin1().trimmed().data(), name.toLatin1().trimmed().data(), list[1].toLatin1().trimmed().data(), list[2].toLatin1().trimmed().data(), list[3].toLatin1().trimmed().data(), list[4].toLatin1().trimmed().data(), filename.toLatin1().data(), MStyleSheetParser::getLineNum(filename, position));
                if (left == 0 && right == 0 && top == 0 && bottom == 0)
                    qWarning("%s: \"%s\";", name.toLatin1().trimmed().data(), name.toLatin1().trimmed().data());
                shm << (int)CONST_SCALABLE_TYPE_5 << name << left << right << top << bottom;
            } else
                shm << (int)CONST_SCALABLE_TYPE_0;
        } else {
            //"background: image_id left right top bottom;"
            //"background: image_id;"
            //"background: "image id" left right top bottom;"
            //"background: "image id";"

            QStringList list;
            if (value.startsWith('\"')) {
                //parse name inside quotes
                int idx = value.indexOf('\"', 1);
                if (idx != -1) {
                    //get quoted image_id
                    QString imageid = value.mid(1, idx - 1);

                    //split rest of the parameters
                    QString values = value.mid(idx + 1).trimmed();
                    list = values.split(' ', QString::SkipEmptyParts);

                    //insert image_id as first parameter
                    list.insert(0, imageid);
                }
            }

            else {
                //no quotes, just split the parameters
                list = value.split(' ');
            }

            //no parameters
            if (value.isEmpty()) {
                //init null image which is ok if someone does not want to use it
                shm << (int)CONST_SCALABLE_TYPE_0;
            }
            //only image_id
            else if (list.size() == 1) {
                shm << (int)CONST_SCALABLE_TYPE_1 << list[0];
            }
            //image_id + border width paramaters
            else if (list.size() == 5) {
                //image_id and the border parameters

                QString name = list[0];
                int left = MStyleSheetAttribute::attributeToInt(list[1], &conversionOK);
                int right = MStyleSheetAttribute::attributeToInt(list[2], &conversionOK);
                int top = MStyleSheetAttribute::attributeToInt(list[3], &conversionOK);
                int bottom = MStyleSheetAttribute::attributeToInt(list[4], &conversionOK);

                shm << (int)CONST_SCALABLE_TYPE_5 << name << left << right << top << bottom;
            }
        }
    }

    else if (attributeType == types[SIZE_TYPE] || attributeType == types[SIZEF_TYPE]) {
        if (value.startsWith(values[SIZE_VALUE], Qt::CaseInsensitive)) {
            //size: size(25px, 25px);

            QString values = value.mid(5, value.length() - 6);
            QStringList list = values.split(',');
            if (list.size() == 2) {
                //since 0.11
                qWarning("DEPRECATED: using \"size\" prefix for size attributes is deprecated. Use following syntax instead:");
                qWarning("%s: %s; => %s: %s %s; //<-- %s:%d", name.toLatin1().trimmed().data(), value.toLatin1().data(), name.toLatin1().trimmed().data(), list[0].toLatin1().trimmed().data(), list[1].toLatin1().trimmed().data(), filename.toLatin1().data(), MStyleSheetParser::getLineNum(filename, position));

                if (attributeType == types[SIZE_TYPE])
                    shm << (int)SIZE_TYPE << list[0] << list[1];
                else
                    shm << (int)SIZEF_TYPE << list[0] << list[1];
            }
        } else {
            //size: 25px 25px;

            //just split into pieces and create QSize or QSizeF depending on the attributeType
            QStringList list = value.split(' ', QString::SkipEmptyParts);
            if (list.size() == 2) {
                if (attributeType == types[SIZE_TYPE])
                    shm << (int)SIZE_TYPE << list[0] << list[1];
                else
                    shm << (int)SIZEF_TYPE << list[0] << list[1];
            }
        }

    } else if (attributeType == types[POINT_TYPE] || attributeType == types[POINTF_TYPE]) {

        if (value.startsWith(values[POS_VALUE], Qt::CaseInsensitive)) {
            //"point: pos(256px, 123px);"

            QString values = value.mid(4, value.length() - 5);
            QStringList list = values.split(',');
            if (list.size() == 2) {
                //since 0.11
                qWarning("DEPRECATED: using \"pos\" prefix for position attributes is deprecated. Use following syntax instead:");
                qWarning("%s: %s; => %s: %s %s; //<-- %s:%d", name.toLatin1().trimmed().data(), value.toLatin1().data(), name.toLatin1().trimmed().data(), list[0].toLatin1().trimmed().data(), list[1].toLatin1().trimmed().data(), filename.toLatin1().data(), MStyleSheetParser::getLineNum(filename, position));
                if (attributeType == types[POINT_TYPE]) {
                    int x = MStyleSheetAttribute::attributeToInt(list[0], &conversionOK);
                    int y = MStyleSheetAttribute::attributeToInt(list[1], &conversionOK);

                    shm << (int)POINT_TYPE << qVariantFromValue(QPoint(x, y));
                } else {
                    qreal x = MStyleSheetAttribute::attributeToFloat(list[0], &conversionOK);
                    qreal y = MStyleSheetAttribute::attributeToFloat(list[1], &conversionOK);

                    shm << (int)POINTF_TYPE << qVariantFromValue(QPointF(x, y));
                }
            }
        } else {
            //"point: 256px 123px;

            //just split into pieces and create QPoint or QPointF depending on the attributeType
            QStringList list = value.split(' ', QString::SkipEmptyParts);
            if (list.size() == 2) {
                if (attributeType == types[POINT_TYPE]) {
                    int width = MStyleSheetAttribute::attributeToInt(list[0], &conversionOK);
                    int height = MStyleSheetAttribute::attributeToInt(list[1], &conversionOK);

                    shm << (int)POINT_TYPE << qVariantFromValue(QPoint(width, height));
                } else {
                    qreal width = MStyleSheetAttribute::attributeToFloat(list[0], &conversionOK);
                    qreal height = MStyleSheetAttribute::attributeToFloat(list[1], &conversionOK);

                    shm << (int)POINTF_TYPE << qVariantFromValue(QPointF(width, height));
                }
            }
        }
    } else if (attributeType == types[FONT_TYPE]) {
        if (value.startsWith(values[FONT_VALUE], Qt::CaseInsensitive)) {
            //"font(sans, 10px);"

            QString values = value.mid(5, value.length() - 6);
            QStringList list = values.split(',');
            if (list.size() == 2) {
                float pixelSize = MStyleSheetAttribute::attributeToFloat(list[1], &conversionOK);
                if (conversionOK) {
                    QFont font(list[0]);
                    font.setPixelSize(pixelSize);
                    //since 0.11
                    qWarning("DEPRECATED: using \"font\" prefix for font is deprecated. Use following syntax instead:");
                    qWarning("%s: %s; => %s: \"%s\" %s; //<-- %s:%d", name.toLatin1().trimmed().data(), value.toLatin1().data(), name.toLatin1().trimmed().data(), list[0].toLatin1().trimmed().data(), list[1].toLatin1().trimmed().data(), filename.toLatin1().data(), MStyleSheetParser::getLineNum(filename, position));

                    shm << (int)FONT_TYPE << font;
                }
            }
        } else {
            //font: "font family" 20px
            //font: "font family" bold 20px
            //font: font_family 20px
            //font: font_family bold 20px

            QStringList list;
            if (value.startsWith('\"')) {
                int idx = value.indexOf('\"', 1);
                if (idx != -1) {
                    //get quoted font family name
                    QString family = value.mid(1, idx - 1);

                    //split rest of the parameters
                    QString values = value.mid(idx + 1).trimmed();
                    list = values.split(' ', QString::SkipEmptyParts);

                    //insert family as first parameter
                    list.insert(0, family);
                }
            } else {
                //no quotes, just split the parameters
                list = value.split(' ');
            }

            //family + font size
            if (list.size() == 2) {
                float pixelSize = MStyleSheetAttribute::attributeToFloat(list[1], &conversionOK);
                if (conversionOK) {
                    QFont font(list[0]);
                    font.setPixelSize(pixelSize);
                    shm << (int)FONT_TYPE << font;
                }
            }
            //family + weight + font size
            else if (list.size() == 3) {
                float pixelSize = MStyleSheetAttribute::attributeToFloat(list[2], &conversionOK);
                if (conversionOK) {
                    QFont font(list[0]);
                    font.setPixelSize(pixelSize);
                    if (DataTypeConverter.WEIGHTS.contains(list[1]))
                        font.setWeight(DataTypeConverter.WEIGHTS[list[1]]);

                    shm << (int)FONT_TYPE << font;
                }
            }
        }
    } else if (attributeType == types[STRING_TYPE]) {
        if (value.length() >= 2) {
            if ((value[0] == 0x22) && (value[value.length()-1] == 0x22)) {
                QString tmp =  value.mid(1, value.length() - 2);
                shm << (int)STRING_TYPE << qVariantFromValue(tmp);
            }
        } else if (value.length() == 0)
            shm << (int)STRING_TYPE << qVariantFromValue(QString());
    }

    else if (attributeType == types[ALIGNMENT_TYPE]) {
        if (DataTypeConverter.ALIGNMENTS.contains(value))
            shm << (int)ALIGNMENT_TYPE << (int)(DataTypeConverter.ALIGNMENTS[value]);
    } else if (attributeType == types[ORIENTATION_TYPE]) {
        if (DataTypeConverter.ORIENTATIONS.contains(value))
            shm << (int)ORIENTATION_TYPE << (int)(DataTypeConverter.ORIENTATIONS[value]);
    } else if (attributeType == types[UNDERLINESTYLE_TYPE]) {
        if (DataTypeConverter.UNDERLINESTYLES.contains(value))
            shm << (int)UNDERLINESTYLE_TYPE << (int)(DataTypeConverter.UNDERLINESTYLES[value]);
    } else if (attributeType == types[PENSTYLE_TYPE]) {
        if (DataTypeConverter.PENSTYLES.contains(value))
            shm << (int)PENSTYLE_TYPE << (int)(DataTypeConverter.PENSTYLES[value]);
    } else if (attributeType == types[AXIS_TYPE]) {
        if (DataTypeConverter.AXES.contains(value))
            shm << (int)AXIS_TYPE << (int)(DataTypeConverter.AXES[value]);
    } else if (attributeType == types[FEEDBACK_TYPE])
        shm << (int)FEEDBACK_TYPE << value;

#if QT_VERSION >= 0x040600
    else if (attributeType == types[EASINGCURVE_TYPE]) {
        QEasingCurve curve;
        // curve type
        QStringList list = value.split(',');
        if (list.size() > 0) {
            if (DataTypeConverter.EASINGCURVETYPES.contains(list[0])) {
                curve.setType(DataTypeConverter.EASINGCURVETYPES[list[0]]);
                // curve amplitude
                if (list.size() > 1) {
                    curve.setAmplitude((qreal) list[1].toDouble());
                    // curve overshoot
                    if (list.size() > 2) {
                        curve.setOvershoot((qreal) list[2].toDouble());
                        // curve period
                        if (list.size() > 3) {
                            curve.setPeriod((qreal) list[3].toDouble());
                        }
                    }
                }

                shm << (int)EASINGCURVE_TYPE << qVariantFromValue(curve);
            }
        }
    } else {
        MStyleSheetParser::outputParseError(filename, "Not a valid attribute(" + attributeType + "): " + name + ": " + value, MStyleSheetParser::getLineNum(filename, position));
    }
#else
    else {
        // This attribute type is supported in Qt4.6->
        shm << (int)NUM_TYPES;
    }
#endif

}

